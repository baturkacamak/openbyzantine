#pragma once
#include <array>
#include <algorithm>
#include <cstdint>
#include <optional>

namespace byzantine {
// Recovered 0x42913b/0x42a579 board, not a scene-name special case.
struct MorrisBoard {
    struct Node { std::array<int,4> peers; int score; std::uint8_t flags; };
    std::array<Node,24> nodes{};
    unsigned turns = 0;
    bool initialized = false;
    void initialize() {
        nodes = {{
            Node{{1,2,9,21},21,4},
            Node{{0,2,4,7},20,0},
            Node{{0,1,14,23},21,4},
            Node{{4,5,10,18},21,4},
            Node{{3,5,1,7},20,0},
            Node{{3,4,13,20},21,4},
            Node{{7,8,11,15},21,4},
            Node{{6,8,1,4},20,0},
            Node{{6,7,12,17},21,4},
            Node{{0,21,10,11},20,0},
            Node{{9,11,3,18},20,0},
            Node{{9,10,6,15},20,0},
            Node{{13,14,8,17},20,0},
            Node{{12,14,5,20},20,0},
            Node{{12,13,2,23},20,0},
            Node{{16,17,6,11},21,4},
            Node{{15,17,19,22},20,0},
            Node{{15,16,8,12},21,4},
            Node{{19,20,3,10},21,4},
            Node{{18,20,16,22},20,0},
            Node{{18,19,5,13},21,4},
            Node{{22,23,0,9},21,4},
            Node{{21,23,16,19},20,0},
            Node{{21,22,2,14},21,4},
        }};
        turns=0;initialized=true;
    }
    static std::uint16_t random(std::uint32_t& state) {
        state=state*0x343fdu+0x269ec3u;
        return static_cast<std::uint16_t>((state>>16)&0x7fff);
    }
    std::optional<unsigned> mill(unsigned owner) const {
        // 0x42a005 / 0x42a10e scan nodes and their two line pairs in order.
        static constexpr int lines[16][3] = {
            {0,1,2},{2,14,23},{21,22,23},{0,9,21},
            {3,4,5},{5,13,20},{18,19,20},{3,10,18},
            {6,7,8},{8,12,17},{15,16,17},{6,11,15},
            {1,4,7},{12,13,14},{16,19,22},{9,10,11}};
        for (unsigned i=0;i<24;++i) {
            if (!(nodes[i].flags&owner)) continue;
            for(int pair=0;pair<2;++pair) {
                int a=nodes[i].peers[pair*2],b=nodes[i].peers[pair*2+1];
                if (!(nodes[a].flags&owner) || !(nodes[b].flags&owner)) continue;
                std::array<int,3> triple{static_cast<int>(i),a,b};
                std::sort(triple.begin(),triple.end());
                for(unsigned line=0;line<16;++line)
                    if(std::equal(triple.begin(),triple.end(),lines[line]))return line;
            }
        }
        return std::nullopt;
    }
    struct Result { std::optional<unsigned> computer; unsigned status=0; unsigned line=0; };
    std::optional<Result> play(unsigned human,std::uint32_t& rng) {
        if(!initialized || human>=24)return std::nullopt;
        Result result;
        nodes[human].flags|=2; // 0x42a9b0; 0x42a990 marks the computer.
        if(auto line=mill(2)) {result.status=2;result.line=*line;return result;}
        int threat=-1;
        for(int i=0;i<24 && threat<0;++i) {
            if(nodes[i].flags&3)continue;
            for(unsigned owner: {1u,2u})for(int pair=0;pair<2;++pair)
                if((nodes[nodes[i].peers[pair*2]].flags&owner) &&
                   (nodes[nodes[i].peers[pair*2+1]].flags&owner))threat=i;
        }
        if(threat>=0)nodes[threat].score+=20;
        else if(nodes[human].flags&4) {
            // 0x42a349: boost empty corner-line peers, then the last corner's peers.
            int last=-1;
            for(int peer:nodes[human].peers)if(nodes[peer].flags&4) {
                last=peer;if(!(nodes[peer].flags&3))++nodes[peer].score;
            }
            if(last>=0)for(int peer:nodes[last].peers)
                if(peer!=static_cast<int>(human) && (nodes[peer].flags&4) && !(nodes[peer].flags&3)) {
                    ++nodes[peer].score;break;
                }
        }
        int maximum=-1,count=0;
        for(const auto& n:nodes)if(!(n.flags&3)) {
            if(n.score>maximum){maximum=n.score;count=1;}
            else if(n.score==maximum)++count;
        }
        if(maximum<0){result.status=3;return result;}
        int choice=1+random(rng)%count;
        for(unsigned i=0;i<24;++i)if(!(nodes[i].flags&3) && nodes[i].score==maximum && --choice==0) {
            nodes[i].flags|=1;result.computer=i;break;
        }
        if(auto line=mill(1)){result.status=1;result.line=*line;}
        if(result.status==0 && ++turns==9)result.status=3;
        return result;
    }
};
} // namespace byzantine
