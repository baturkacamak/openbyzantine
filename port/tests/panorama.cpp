#include "byzantine/panorama.hpp"

#include <iostream>
#include <cmath>
#include <cfenv>
#include <limits>
#include <stdexcept>
#include <utility>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main() {
    try {
        for (const auto& sample : {std::pair<double, int>{0.5, 0}, {1.5, 2},
                {2.5, 2}, {-0.5, 0}, {-1.5, -2}, {-2.5, -2},
                {0.5001, 1}, {-0.5001, -1}, {0.4999, 0}, {-0.4999, 0},
                {2147483647.0, 2147483647}, {-2147483648.0, (-2147483647 - 1)}})
            require(byzantine::panorama_table_coordinate(sample.first) == sample.second,
                    "table bias nearest/even conversion");
        for (double bad : {2147483647.5, -2147483649.0,
                           std::numeric_limits<double>::infinity(),
                           std::numeric_limits<double>::quiet_NaN()}) {
            bool failed = false;
            try { byzantine::panorama_table_coordinate(bad); }
            catch (const std::runtime_error&) { failed = true; }
            require(failed, "invalid table conversion accepted");
        }
        const byzantine::PanoramaLookupTables tables;
        // Reset is an explicit operation with known dimensions, not a default
        // for an unknown saved game. Check the recovered +pi starting yaw.
        byzantine::PanoramaProjection projection(tables, 64, 16, 2, 4, 2);
        const auto reset_map = projection.mapping();
        require(projection.camera().yaw_radians == 3.14159265359 &&
                projection.camera().pitch_radians == 0 &&
                projection.camera().zoom == 1 && !projection.camera().at_pitch_limit &&
                projection.yaw_word_offset() == 32,
                "projection reset camera");
        const auto* cached = projection.mapping().data();
        require(!projection.advance(tables, 0, 0, 1) &&
                projection.mapping().data() == cached && projection.mapping() == reset_map,
                "neutral draw rebuilt cache");
        require(!projection.advance(tables, 180, 0, 1) &&
                projection.camera().yaw_radians == 0 &&
                projection.mapping().data() == cached,
                "yaw-only draw regenerated mapping/wrapped incorrectly");
        // 0x4244f4..0x42451f converts degrees with fmul pi (0x45b160) then
        // fdiv 180 (0x45b180). CRT startup installs PC=53, so each step is a
        // binary64 store. -180 degrees therefore lands one ulp BELOW -pi and
        // the strict `< -pi` wrap immediately adds 2*pi. A PC64 model would
        // land exactly on -pi and stay negative, so these bits distinguish the
        // two models; exact -pi is not reachable through a degree delta.
        require(!projection.advance(tables, -180, 0, 1) &&
                projection.camera().yaw_radians == 0x1.921fb54442ee9p+1 &&
                !projection.advance(tables, -360, 0, 1) &&
                projection.camera().yaw_radians == 0x1.921fb54442ee7p+1,
                "negative yaw delta/wrap must follow PC53 binary64 steps");
        require(projection.advance(tables, 0, -360, 1) &&
                projection.camera().pitch_radians > 0 && projection.camera().at_pitch_limit,
                "positive pitch limit/sign");
        require(!projection.advance(tables, 0, 0, 1) && projection.camera().at_pitch_limit,
                "neutral input cleared limit flag");
        require(projection.advance(tables, 0, 1, 1) && !projection.camera().at_pitch_limit,
                "interior pitch did not clear limit flag");
        require(projection.advance(tables, 0, 360, 1) &&
                projection.camera().pitch_radians < 0 && projection.camera().at_pitch_limit,
                "negative pitch limit/sign");
        projection.reset(tables);
        require(projection.camera().yaw_radians == 3.14159265359 &&
                projection.camera().pitch_radians == 0 &&
                projection.camera().zoom == 1 && !projection.camera().at_pitch_limit &&
                projection.mapping() == reset_map, "reset did not restore camera/map");
        // 0x4236b5 load path: geometry is dimension-dependent, but the camera
        // reset (+0x4241e2) runs for every successful decoded image.
        const auto ea = byzantine::panorama_geometry(2496, 768, 640, 480);
        require(ea.stride_words == 3104 && ea.padding == 304,
                "recovered source stride/geometry");
        byzantine::PanoramaProjection loaded(tables, 2496, 768, ea.padding, 640, 480);
        require(loaded.padding() == 304 && loaded.stride_words() == 3104,
                "loaded projection geometry");
        require(!loaded.advance(tables, 747.0 * 360 / 2496, 0, 1) &&
                loaded.camera().yaw_radians != 3.14159265359,
                "load-path camera precondition");
        require(!loaded.set_source(tables, 2496, 768) &&
                loaded.padding() == 304 && loaded.stride_words() == 3104 &&
                loaded.camera().yaw_radians == 3.14159265359 &&
                loaded.camera().pitch_radians == 0 && loaded.camera().zoom == 1 &&
                !loaded.camera().at_pitch_limit &&
                loaded.mapping() == byzantine::panorama_mapping(tables,2496,768,304,640,480,0,1),
                "same-size reload must reset camera but keep geometry");
        require(loaded.set_source(tables, 2496, 480) &&
                loaded.padding() == 270 && loaded.stride_words() == 3036 &&
                loaded.camera().yaw_radians == 3.14159265359 &&
                loaded.mapping() == byzantine::panorama_mapping(tables,2496,480,270,640,480,0,1),
                "dimension change must recompute geometry and reset");
        const auto before_bad = loaded.mapping();
        bool bad_source_rejected = false;
        try { loaded.set_source(tables, 0, 480); }
        catch (const std::runtime_error&) { bad_source_rejected = true; }
        require(bad_source_rejected && loaded.padding() == 270 &&
                loaded.stride_words() == 3036 && loaded.mapping() == before_bad,
                "invalid reload accepted or mutated state");
        // Reloading identical dimensions still resets a moved camera.
        require(!loaded.advance(tables, 90, 0, 1) &&
                loaded.camera().yaw_radians != 3.14159265359,
                "load-path yaw move");
        require(!loaded.set_source(tables, 2496, 480) &&
                loaded.camera().yaw_radians == 3.14159265359 &&
                loaded.mapping() == before_bad,
                "same-size reload did not reset the camera");
        for (double factor : {0.5, 0.0, -2.0})
            require(projection.advance(tables, 0, 0, factor) &&
                    projection.camera().zoom == 1 && projection.mapping() == reset_map,
                    "zoom floor skipped mandatory regeneration");
        require(projection.advance(tables, 0, 0, 2) && projection.camera().zoom == 2 &&
                projection.mapping() == byzantine::panorama_mapping(tables,64,16,2,4,2,0,2),
                "zoom cache mismatch");
        require(projection.advance(tables, 0, -5, 1) &&
                std::abs(projection.camera().pitch_radians - 5*3.14159265359/180) < 1e-16 &&
                projection.mapping() == byzantine::panorama_mapping(tables,64,16,2,4,2,
                    projection.camera().pitch_radians,2), "pitch cache mismatch");
        const auto stable = projection.camera();
        const auto stable_map = projection.mapping();
        for (const auto delta : {std::array<double,3>{361,0,1}, {0,0,1e308},
                {0,1e308,1}, {0,std::numeric_limits<double>::quiet_NaN(),1},
                {0,0,std::numeric_limits<double>::infinity()}}) {
            bool failed = false;
            try { projection.advance(tables,delta[0],delta[1],delta[2]); }
            catch (const std::runtime_error&) { failed = true; }
            const auto after = projection.camera();
            require(failed && after.yaw_radians == stable.yaw_radians &&
                    after.pitch_radians == stable.pitch_radians && after.zoom == stable.zoom &&
                    after.at_pitch_limit == stable.at_pitch_limit &&
                    projection.mapping() == stable_map, "failed update changed state/cache");
        }
        require(byzantine::clamp_panorama_pitch(0.25,0.25L).at_limit &&
                byzantine::clamp_panorama_pitch(-0.25,0.25L).at_limit &&
                !byzantine::clamp_panorama_pitch(0,0.25L).at_limit &&
                byzantine::clamp_panorama_pitch(0,0).at_limit,
                "pitch clamp equality/interior");
        // Synthetic live return slightly ABOVE a binary64 value: upper
        // equality with the STORE is interior; lower equality is clamped.
        if (std::numeric_limits<long double>::digits > 53) {
            const long double live = 0.25L + std::ldexp(1.0L,-58);
            require(static_cast<double>(live) == 0.25 &&
                    !byzantine::clamp_panorama_pitch(0.25,live).at_limit &&
                    byzantine::clamp_panorama_pitch(-0.25,live).at_limit,
                    "live return vs binary64 limit distinction lost");
        }
        const int projection_rounding = std::fegetround();
        require(std::fesetround(FE_DOWNWARD) == 0, "set projection test rounding");
        bool projection_rejected_rounding = false;
        try { projection.advance(tables,0,0,1); }
        catch (const std::runtime_error&) { projection_rejected_rounding = true; }
        const bool projection_preserved_rounding = std::fegetround() == FE_DOWNWARD;
        std::fesetround(projection_rounding);
        require(projection_rejected_rounding && projection_preserved_rounding &&
                projection.mapping() == stable_map, "projection rounding contract");
        // Hand-checkable small map: zero pitch, unit zoom, small angles round
        // to dx=-2..2 and dy=-1..1. Entries are WORD, not byte, offsets.
        require(byzantine::panorama_mapping(tables, 64, 16, 2, 5, 3, 0, 1) ==
            std::vector<std::int32_t>{508,509,510,511,512, 576,577,578,579,580,
                                      644,645,646,647,648}, "small mapping word offsets");
        const auto odd_map = byzantine::panorama_mapping(tables, 65, 17, 2, 5, 3, 0, 1);
        require(odd_map.front() == 515 && odd_map[7] == 586 && odd_map.back() == 657,
                "mapping integer halves and padded stride");
        const auto zoom_map = byzantine::panorama_mapping(tables, 64, 16, 2, 5, 3, 0, 2);
        require(zoom_map == std::vector<std::int32_t>{577,578,578,578,579,
                    577,578,578,578,579, 577,578,578,578,579},
                "zoom must contract around source center");
        require(byzantine::panorama_mapping(tables, 64, 16, 2, 1, 1, 0.1, 1)[0] == 646 &&
                byzantine::panorama_mapping(tables, 64, 16, 2, 1, 1, -0.1, 1)[0] == 510,
                "mapping pitch sign");
        const auto mapping_rejected = [&](int sw, int sh, int pad, int vw, int vh,
                                          double pitch, double zoom) {
            try { byzantine::panorama_mapping(tables, sw, sh, pad, vw, vh, pitch, zoom); }
            catch (const std::runtime_error&) { return true; }
            return false;
        };
        require(mapping_rejected(64, 16, 2, 0, 3, 0, 1) &&
                mapping_rejected(64, 16, 65, 5, 3, 0, 1) &&
                mapping_rejected(64, 16, 2, 5, 3, 0, 0.5) &&
                mapping_rejected(64, 16, 2, 5, 3,
                    std::numeric_limits<double>::quiet_NaN(), 1) &&
                mapping_rejected(64, 16, 2, 5, 3, 3.14159265359, 1) &&
                mapping_rejected(64, 16, 2, 5, 3, 0, 1e308) &&
                mapping_rejected(std::numeric_limits<int>::max(),
                    std::numeric_limits<int>::max(), std::numeric_limits<int>::max(),
                    1, 1, 0, 1), "invalid mapping state accepted");
        require(mapping_rejected(64, 16, 2, 43, 1, 0, 1),
                "mapping accepted uninitialized atan edge");
        require(mapping_rejected(64, 2, 2, 1, 5, 0, 1),
                "mapping accepted out-of-source row");
        const int saved_rounding = std::fegetround();
        require(std::fesetround(FE_DOWNWARD) == 0, "set test rounding mode");
        const bool wrong_rounding_rejected = mapping_rejected(64, 16, 2, 5, 3, 0, 1);
        const bool mode_preserved = std::fegetround() == FE_DOWNWARD;
        std::fesetround(saved_rounding);
        require(wrong_rounding_rejected && mode_preserved, "mapping rounding mode contract");
        require(tables.cosine(0) == 1 && tables.arctangent(0) == 0 &&
                std::signbit(tables.arctangent(0)), "table zero overwrite/sign");
        for (int i = 1; i <= 1569; ++i)
            require(tables.cosine(i) == tables.cosine(-i) &&
                    tables.cosine(i) <= tables.cosine(i - 1) &&
                    tables.cosine(i) > 0, "cosine table symmetry/range");
        for (int i = 1; i <= 1999; ++i)
            require(tables.arctangent(i) == -tables.arctangent(-i) &&
                    tables.arctangent(i) > tables.arctangent(i - 1),
                    "arctangent table symmetry/range");
        require(std::abs(tables.cosine(1569) - 0.0017963258f) < 1e-9f &&
                std::abs(tables.arctangent(1999) - 1.1069486f) < 1e-7f,
                "table endpoint argument scaling");
        for (int index : {-1571, -1570, 1570, 1571}) {
            bool failed = false;
            try { tables.cosine(index); }
            catch (const std::runtime_error&) { failed = true; }
            require(failed, "uninitialized cosine cell exposed");
        }
        for (int index : {-2000, 2000}) {
            bool failed = false;
            try { tables.arctangent(index); }
            catch (const std::runtime_error&) { failed = true; }
            require(failed, "uninitialized arctangent cell exposed");
        }
        // Synthetic, explicitly supplied viewports; no game viewport assumed.
        const auto geometry = byzantine::panorama_geometry(2496, 768, 640, 480);
        std::vector<std::uint16_t> full_source(3104 * 768);
        for (std::size_t i = 0; i < full_source.size(); ++i)
            full_source[i] = static_cast<std::uint16_t>((i / 3104) * 37 + i % 3104);
        for (double pitch : {0.0, geometry.pitch_limit_radians,
                              -geometry.pitch_limit_radians}) {
            const auto map = byzantine::panorama_mapping(tables, 2496, 768,
                geometry.padding, 640, 480, pitch, 1);
            require(map.size() == 640 * 480, "full viewport mapping size");
            for (auto offset : map)
                require(offset >= 0 && offset < 3104 * 768,
                        "full viewport mapping outside padded buffer");
            for (double yaw : {-3.14159265359, 3.14159265359}) {
                const int shift = byzantine::panorama_yaw_offset(geometry.pixels_per_radian, yaw);
                std::vector<std::uint16_t> output(644 * 482, 0x5aa5);
                byzantine::sample_panorama_rgb16(full_source, map, shift, output,
                                                {1288, 2, 1, 640, 480}, 0);
                for (int row = 0; row < 482; ++row) {
                    for (int column = 0; column < 644; ++column) {
                        if (row == 0 || row == 481 || column < 2 || column >= 642)
                            require(output[row * 644 + column] == 0x5aa5,
                                    "full sampling overwrote rectangle border");
                        else {
                            const auto source_offset = map[(row - 1) * 640 + column - 2] + shift;
                            require(output[row * 644 + column] == full_source.at(source_offset),
                                    "full sampling pitch/yaw address mismatch");
                        }
                    }
                }
            }
            if (pitch == 0) {
                require(map[240 * 640 + 320] == 384 * 3104 + 1552,
                        "centered full viewport mapping");
                for (int x = 1; x < 320; ++x) {
                    const auto left = map[240 * 640 + 320 - x];
                    const auto right = map[240 * 640 + 320 + x];
                    require(left + right == 2 * map[240 * 640 + 320],
                            "zero-pitch horizontal map symmetry");
                }
            }
        }
        require(geometry.padding == 304 &&
                std::abs(geometry.pitch_limit_radians - 0.22497169348767235) < 1e-14,
                "recovered panorama seam geometry");
        const auto odd = byzantine::panorama_geometry(2496, 769, 641, 481);
        require(odd.padding == geometry.padding &&
                odd.pitch_limit_radians == geometry.pitch_limit_radians,
                "geometry must use integer half dimensions");
        require(byzantine::panorama_geometry(2496, 480, 640, 480).padding == 270 &&
                byzantine::panorama_pitch_limit(geometry.pixels_per_radian,
                                                480, 480, 1) == 0,
                "equal-height pitch limit");
        require(byzantine::panorama_geometry(2496, 768, 320, 200).padding == 198 &&
                byzantine::panorama_pitch_limit(geometry.pixels_per_radian,
                    768, 480, 2) > geometry.pitch_limit_radians,
                "viewport/zoom geometry");
        require(byzantine::panorama_angle_degrees(3.14159265359) == 180 &&
                byzantine::panorama_angle_degrees(-3.14159265359) == -180,
                "original angle constant");
        require(byzantine::panorama_yaw_offset(10, 0.19) == 1 &&
                byzantine::panorama_yaw_offset(10, -0.19) == -1 &&
                byzantine::panorama_yaw_offset(10, -0.09) == 0,
                "yaw must truncate toward zero, not floor or round");
        for (double bad : {0.0, -1.0, std::numeric_limits<double>::infinity(),
                           std::numeric_limits<double>::quiet_NaN()}) {
            bool failed = false;
            try { byzantine::panorama_yaw_offset(bad, 1); }
            catch (const std::runtime_error&) { failed = true; }
            require(failed, "invalid yaw scale accepted");
        }
        bool overflow_rejected = false;
        try { byzantine::panorama_yaw_offset(1e300, 1e300); }
        catch (const std::runtime_error&) { overflow_rejected = true; }
        require(overflow_rejected, "overflowing yaw conversion accepted");
        // EA's 2496-wide asset: the script's x=747 request becomes 108 degrees.
        for (bool alternate : {false, true}) {
            int pending = 747;
            const auto setup = byzantine::consume_panorama_position(pending, 2496, alternate);
            require(setup.horizontal_degrees == 108 &&
                    setup.vertical_degrees == (alternate ? -6 : 0) &&
                    setup.reset_projection && pending == -1, "EA camera consumption");
            const auto again = byzantine::consume_panorama_position(pending, 2496, alternate);
            require(again.horizontal_degrees == 0 && again.vertical_degrees == 0 &&
                    !again.reset_projection, "camera request applied twice");
        }
        for (const auto& pair : {std::pair<int, int>{0, 0}, {1248, 180}, {2495, 360}}) {
            int pending = pair.first;
            require(byzantine::consume_panorama_position(pending, 2496, false)
                        .horizontal_degrees == pair.second, "camera endpoint rounding");
        }
        int half = 1;
        require(byzantine::consume_panorama_position(half, 720, false)
                    .horizontal_degrees == 1, "half degree must round upward");
        int invalid = 747;
        bool rejected = false;
        try { byzantine::consume_panorama_position(invalid, 0, false); }
        catch (const std::runtime_error&) { rejected = true; }
        require(rejected && invalid == 747, "failed setup consumed camera request");

        // Distinct rows catch wrong seam direction and cross-row copying;
        // high bits ensure that the helper preserves full 16-bit pixel words.
        const std::vector<std::uint16_t> pixels{0x8001, 2, 3, 4, 11, 12, 13, 0xffff};
        require(byzantine::pad_panorama_rows(pixels, 4, 2, 2) ==
                    std::vector<std::uint16_t>{3, 4, 0x8001, 2, 3, 4, 0x8001, 2,
                                               13, 0xffff, 11, 12, 13, 0xffff, 11, 12},
                "panorama seam layout");
        require(byzantine::pad_panorama_rows(pixels, 4, 2, 0) == pixels,
                "zero padding changed image");
        const auto padded = byzantine::pad_panorama_rows(pixels, 4, 2, 2);
        const std::vector<std::int32_t> sample_map{2, 3, 10, 11};
        const byzantine::PanoramaDestination rect{14, 2, 1, 2, 2};
        for (int yaw : {0, 1, -1}) {
            std::vector<std::uint16_t> output(28, 0x5aa5), expected = output;
            if (yaw == 0) {
                expected[9] = 0x8001; expected[10] = 2;
                expected[16] = 11; expected[17] = 12;
            } else if (yaw == 1) {
                expected[9] = 2; expected[10] = 3;
                expected[16] = 12; expected[17] = 13;
            } else {
                expected[9] = 4; expected[10] = 0x8001;
                expected[16] = 0xffff; expected[17] = 11;
            }
            byzantine::sample_panorama_rgb16(padded, sample_map, yaw, output, rect, 0);
            require(output == expected, "RGB16 word/yaw/rectangle/pitch sampling");
        }
        // First/last valid final word, and the original's flat-address addition
        // across a source row boundary (there is no per-row modulo or clamp).
        std::vector<std::uint16_t> edge_output(2);
        byzantine::sample_panorama_rgb16(padded, {0, 15}, 0, edge_output,
                                        {4, 0, 0, 2, 1}, 0);
        require(edge_output == std::vector<std::uint16_t>{3, 12}, "source endpoint sampling");
        byzantine::sample_panorama_rgb16(padded, {7, 14}, 1, edge_output,
                                        {4, 0, 0, 2, 1}, 0);
        require(edge_output == std::vector<std::uint16_t>{13, 12}, "flat yaw addressing");
        const auto sampling_rejected = [&](std::vector<std::int32_t> map, int yaw,
                byzantine::PanoramaDestination area, int effect, std::size_t size = 28) {
            std::vector<std::uint16_t> output(size, 0x5aa5);
            const auto before = output;
            try { byzantine::sample_panorama_rgb16(padded, map, yaw, output, area, effect); }
            catch (const std::runtime_error&) {
                require(output == before, "failed sampler partially modified destination");
                return true;
            }
            return false;
        };
        require(sampling_rejected(sample_map, 0, rect, 1) &&
                sampling_rejected(sample_map, 0, rect, -1) &&
                sampling_rejected(sample_map, 0, {13, 2, 1, 2, 2}, 0) &&
                sampling_rejected(sample_map, 0, {-14, 2, 1, 2, 2}, 0) &&
                sampling_rejected(sample_map, 0, {14, 6, 1, 2, 2}, 0) &&
                sampling_rejected(sample_map, 0, {14, -1, 1, 2, 2}, 0) &&
                sampling_rejected(sample_map, 0, {14, 2, 1, 0, 2}, 0) &&
                sampling_rejected(sample_map, 0, rect, 0, 17) &&
                sampling_rejected({2, 3, 10}, 0, rect, 0),
                "invalid sampler layout/effect accepted");
        require(sampling_rejected({2, 3, 10, 16}, 0, rect, 0) &&
                sampling_rejected({2, 3, 10, 15}, 1, rect, 0) &&
                sampling_rejected({2, 3, 10, 0}, -1, rect, 0) &&
                sampling_rejected({2, 3, 10, -1}, 0, rect, 0) &&
                sampling_rejected(sample_map, std::numeric_limits<int>::max(), rect, 0) &&
                sampling_rejected({2, 3, 10, std::numeric_limits<int>::max()},
                                   1, rect, 0) &&
                sampling_rejected(sample_map, 0,
                    {2147483646, 0, 2147483647, 2, 2}, 0),
                "invalid final yaw/32-bit offset accepted");
        auto alias = padded;
        rejected = false;
        try { byzantine::sample_panorama_rgb16(alias, sample_map, 0, alias, rect, 0); }
        catch (const std::runtime_error&) { rejected = true; }
        require(rejected && alias == padded, "sampler accepted aliased buffers");
        require(byzantine::pad_panorama_rows({7}, 1, 1, 1) ==
                    std::vector<std::uint16_t>{7, 7, 7}, "single pixel seam");
        for (const auto width : {std::size_t{0}, std::size_t{3},
                                 std::numeric_limits<std::size_t>::max()}) {
            rejected = false;
            try { byzantine::pad_panorama_rows(pixels, width, 2, 2); }
            catch (const std::runtime_error&) { rejected = true; }
            require(rejected, "accepted invalid/overflowing row dimensions");
        }
        rejected = false;
        try { byzantine::pad_panorama_rows(pixels, 4, 2, 5); }
        catch (const std::runtime_error&) { rejected = true; }
        require(rejected, "accepted seam larger than source row");
        std::cout << "panorama camera/seam/projection tests passed\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
