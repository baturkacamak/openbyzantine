# Kütüphane stratejisi — mevcut durum, riskler ve yol haritası

26 Ekim 2026 · Kod değişikliği yok; yalnızca `port/` kaynakları, `Makefile`/
`CMakeLists.txt` ve gemideki birkaç medya dosyasının başlığı (ffprobe/7z)
incelendi.

Etiketler: **[doğrulandı]** = kodu/veriyi okuyarak veya çalıştırarak görüldü,
**[tahmin]** = çıkarım, **[denenmedi]** = bu ortamda sınanmadı.

## Kısa özet

Bugüne kadar "hazır bir kütüphanenin yapabileceği" işlerden **gerçekten elle
yazdığımız tek büyük parça JPEG çözücü**. Geri kalan her şey ya oyuna özgü
biçimler (RLE, ALL, CC/CB, save, anims) ya da 30–60 satırlık çıktı yazıcılar
(PPM/TGA) ve küçük piksel dönüşümleri. **Ses, video ve metin/yazı tipi tarafında
hiç kodumuz yok**; ses/video yalnızca "istek" kaydı olarak duruyor.

Bağlı tek hazır kütüphane **SDL2** (yalnızca `byzantine-play`). **ffmpeg bağlı
değil**, oynatıcıda POSIX `fork/exec` ile dış süreç olarak çağrılıyor. Analiz
tarafında `7z` ve `python3` dış süreç.

Bugünkü en önemli yapısal boşluk: `byzantine-play` `compose.cpp`'yi **link ediyor
ama hiç çağırmıyor** — yani sprite'ları çizmiyor, sadece arka plan + hotspot
dikdörtgenleri gösteriyor. Bu, "oynanabilir" hedefine en yakın somut eşik.

## 1) Elle yazdığımız, aslında hazır kütüphane işi olan parçalar

**[doğrulandı]**

- `port/src/jpeg.cpp` + `port/include/byzantine/jpeg.hpp` — baseline+progressive
  JPEG: Huffman, DQT/DHT/DRI, IDCT, kroma örnekleme, YCbCr→RGB, RGB565.
  **Tek gerçek "kütüphane işi" bu.**
- `port/src/compose.cpp` — `write_ppm` (P6) ve `blit_frame`/`blit_image`
  (maskeli, kırpan blit).
- `port/src/main.cpp` — `write_tga` (32-bit BGRA TGA yazıcı).
- `port/src/rle.cpp` — oyunun RGB555 RLE kare çözücüsü + 555→888 genişletme
  (biçim oyuna özgü; dönüşüm önemsiz).
- `port/src/anims.cpp` — kare run-stream çözücü, `0x4164a0` **555→565**
  dönüşümü, `parse_avi_anims` (sadece **RIFF/AVI başlık** ayrıştırma, kod çözme
  yok).
- `port/src/panorama.cpp` — sinüs/kosinüs/arctan tabloları, yaw/pitch, PC53/RC
  yuvarlama modeli (`<cmath>` kullanıyor ama niceleme/yuvarlama kuralları elle).
- `port/src/cc.cpp` (~satır 630) — CRT `rand` LCG'si (`0x343fd`, `0x269ec3`);
  standart algoritma, ama birebir dizi için elle.
- `port/tests/jpeg.cpp` — test için **elle JPEG üreten** bit-writer (yalnızca
  test).

**[doğrulandı] Hiç yazmadığımız alanlar:** ses çalma/kod çözme, video kod çözme,
yazı tipi/metin çizimi, PNG kodlama, sıkıştırma (ALL arşivinde sıkıştırma yok,
sadece indeks).

## 2) Kullandığımız hazır kütüphaneler ve bağlantı biçimi

| Araç | Nasıl kullanılıyor | Durum |
| --- | --- | --- |
| **SDL2** | `port/src/play_main.cpp`: pencere, renderer, olaylar, `SDL_LoadBMP`, dikdörtgen çizimi | **Bağlı** (`Makefile` satır 18 pkg-config, `CMakeLists.txt` `SDL2::SDL2`) — yalnızca oynatıcı |
| **ffmpeg** | `play_main.cpp` `decode_jpeg()`: `fork`/`pipe`/`execlp("ffmpeg", … "-vf","scale=640:400","-pix_fmt","rgb24")` | **Dış süreç**, POSIX'e bağımlı; Windows'ta derlenmez |
| **7z** | `analysis/rea/build_vm_census.py`, `verify_panorama_bindings.py` | Analiz zamanı, dış süreç |
| **python3** | aynı betikler + `build_source_corpus.py`, `audit_port_coverage.py` | Analiz zamanı |
| ffmpeg (bu adımda) | **Sadece doğrulama oracle'ı** (JPEG karşılaştırması), `/tmp`'de | Depoya girmedi |

**[doğrulandı]** `port/src` ve `port/include` içinde SDL dışında hiçbir üçüncü
taraf başlığı yok; başka bir bağlı kütüphane de yok.

## 3) Neden yeniden yazdım?

Dürüst ayrım:

- **JPEG: zorunluluk değil, tercih.** Oyuna özgü bir gereksinim yok;
  libjpeg-turbo da "doğru bir görüntü" verirdi. Gerekçelerim: (a) sıfır dış
  bağımlılık ve tek `make` ile üç platformda derlenebilme, (b) bozuk akışı
  **hata** sayan katı davranış, (c) determinist çıktı (test hash'leri). Buna
  karşılık özgünle **piksel birebirliği kanıtlanamaz** — orijinal motor hiç
  çalıştırılmadı; libjpeg 6a'nın algoritmalarını taklit ettim, ölçüm değil
  çıkarım. Yani "hazır kütüphane yeterdi" itirazı geçerli olurdu; karşılığında
  hız ve olgunluk kaybını kabul ettim (ölçtüğüm: 283 JPEG / 11.3 s, ihtiyaçtan
  fazlasıyla hızlı).
- **Diğerleri: kütüphane işi değil.** RLE/ALL/CC/CB/save/anims biçimlerini
  hiçbir hazır kütüphane bilmez. Panorama matematiği ise **birebir sonuç** için
  gerekliydi; genel amaçlı bir trig/matris kütüphanesi `0x4245a5`'in PC53
  yuvarlamasını üretmez (bkz. `PANORAMA_PRECISION_FIX.md`).
- **PPM/TGA yazıcıları** için kütüphane gereksizdi; ama bunlar "yanlış karar"
  değil, 30 satırlık teşhis çıktısı.

## 4) Standart iş ile oyuna özgü kurallar ne kadar ayrı?

**[doğrulandı] Biçim düzeyinde ayrım temiz:** her biçim tek `.hpp/.cpp` çifti ve
dar bir API (`parse_cc`, `decode_rle`, `decode_jpeg`, `load_save`,
`decode_anims_frame`, `compose_scene`). JPEG tamamen kendi dosyasında.

**Ama ortak bir "soyutlama katmanı" yok:** yüzey bizim `Rgb16Surface`'imiz
(SDL_Surface değil), blit bizim, ses/video/metin katmanı hiç yok.

Sonuç:

- **JPEG değişimi = dosya-yerel**: `jpeg.cpp` + `jpeg.hpp` + Makefile/CMake
  satırı. Testler zaten `decode_jpeg`/`rgb_to_rgb565` API'sine bakıyor.
- **Renderer değişimi = motor geneli**: `compose.cpp`, `play_main.cpp` ve çizim
  kuralları (kırpma, kapsama maskesi, sıralama) birlikte değişir; yeniden
  doğrulama gerekir.
- **Ses/video eklemek = refactor değil, yeni katman**: `SceneSoundSession`/video
  istek yapıları zaten veri olarak var; kütüphane bunların arkasına konabilir.

## 5) Bundan sonra hangi iş için hangi kütüphane (Win/macOS/Linux, C++17)

Gemideki gerçek biçimler ölçüldü: **[doğrulandı]** sesler `sounds/*.WAV` =
**PCM s16le, 22050 Hz, mono veya stereo**; video dosyaları `dialog/*.avi` =
**Indeo 4** 320x200@15 + PCM ses, `anims/BCKILL1.AVI` = **Indeo 3**,
`anims/VEGE2.AVI` = Indeo 4.

| İş | Öneri | Platform | Özel kodda kalması gereken |
| --- | --- | --- | --- |
| Pencere/girdi | **SDL2** (zaten var) veya SDL3 | üçünde de | — |
| JPEG (isteğe bağlı) | **libjpeg-turbo** (CMake, BSD) | üçünde de | `0x40f080` RGB→565 daraltması; bizim çözücü oracle olarak kalır |
| Ekran görüntüsü/PNG | `stb_image_write` (tek başlık) veya libpng; SDL_SaveBMP de yeter | üçünde de | — |
| Ses çalma | **miniaudio** (tek başlık, bağımlılıksız) veya SDL2 audio | üçünde de | hangi id ne zaman, öncelik/loop, **konuşma/`0x428cbd` oturumlarının bloklaması** |
| WAV okuma | `dr_wav` veya ~80 satır RIFF walk | üçünde de | — (biçim zaten PCM) |
| Video | **FFmpeg libav\*** (LGPL derleme) — Indeo 3/4 için tek gerçekçi seçenek | üçünde de | başlat/bitir zamanlaması, geçiş kodu (`0x432133`), **bitince betiğin devamı** |
| Metin | yazı tipi varlığı belirlenene kadar **özel**; TTF çıkarsa `stb_truetype` | — | satır kırma, `0x417a17` istek modeli |

**Özel kalması gerekenler:** `cc.cpp` (VM), `cb/cc/save/records/variables`
ayrıştırıcıları, `panorama.cpp`, `compose.cpp`'nin piksel kuralları,
`anims.cpp`/`rle.cpp` çözücüleri.

## 6) Yeni kütüphaneler davranışı nerede bozabilir?

- **Renkler:** libjpeg'in `JCS_RGB565` çıkışı bizim `r>>3,g>>2,b>>3`
  daraltmamızdan farklı bit düzeni/yuvarlama kullanır → görünür ton kayması.
  turbo'nun SIMD IDCT'si ±1 oynar → hash testleri kırılır. Ayrıca `0x4164a0`'ın
  555→565'i bizim kalmalı.
- **Saydamlık:** sprite saydamlığı **renk anahtarı değil**, RLE akışından gelen
  **kapsama maskesi** (komut 3). `SDL_SetColorKey`/alpha blending kenarları
  değiştirir; üstelik `+0x09 & 0x80` karışımı (`0x42b5b2`) henüz modellenmediği
  için kütüphane karışımı **eksik bir kuralı gizler**.
- **Koordinatlar:** playfield kayması `y-0x14`, `+0x20` snap dikdörtgeni,
  panorama dolgulu stride + yaw kelime ofseti, `0x40241c`'nin "sol-üst pencere
  dışındaysa hiç çizme" kuralı. Kütüphanede ölçekleme (mevcut
  `ffmpeg scale=640:400` gibi) örneklemeyi değiştirir; 1:1 çözülmeli.
- **Ses/video zamanlaması:** özgün motor çalarken betiği bekletiyor
  (`timeGetTime`, oturum timeout'ları, AVI `dwLength`). Asenkron bir kütüphane
  oynatıcısı betiği öne koşturur → diyalog sırası ve çıkışlar bozulur.
- **Oynatma bitince devam:** bitiş olayı VM'i ilerletmeli; kütüphane olayı
  yutarsa betik sonsuza kadar bekler.

**Doğrulama planı (hepsi mevcut altyapıyla yapılabilir):**

1. `jpeg-test` düzlem hash'leri + **kütüphane vs bizim çözücü** 283 dosyada
   ortalama/maks fark (eşitlik değil, tolerans raporu).
2. Altın kareler: MAP 87 (`pixels=163079`), NB 556, VT 1638 — piksel sayısı +
   sağlama.
3. Davranış değişmezi: `--trace`/`--census` aynı durak komutu ve aynı sayaçlar
   (completed 5,935, blocking 0).
4. Zamanlama için **sahte saat** enjeksiyonu + "oynatma bitene kadar bekle"
   durumu; test determinist olur.
5. Her değişim için kısa kontrol listesi: renk (565 değerleri), saydamlık
   (maske kenarları), koordinat (çizim dikdörtgenleri), zamanlama (adım sırası).

## 7) Mevcut çalışan kodu değiştirmek işi azaltır mı?

- **Koru:** `jpeg.cpp` (çalışıyor, katı, bağımlılıksız, doğrulanmış), tüm
  ayrıştırıcılar, VM, `panorama.cpp`, `compose.cpp`'nin piksel kuralları. Bunları
  değiştirmek bugün **yeni doğrulama yükü** getirir, hız kazancı getirmez.
- **Değiştir (gerçek kazanç):** `play_main.cpp` — ffmpeg `fork`'u kaldır,
  `decode_jpeg_file` kullan, `compose_scene` ile kayıtları çiz. Bu dosya hem
  POSIX'e bağımlı hem de oynatıcının asıl eksiği.
- **Kütüphaneyle tamamla:** ses çalma (miniaudio/SDL audio), video (FFmpeg), PNG
  ekran görüntüsü (isteğe bağlı). Yani tasarruf, **var olanı değiştirmekte
  değil, henüz yazılmamış alt sistemleri yazmamakta**.

## 8) Oynanabilirliğe en yakın somut hedef ve üç adım

**Hedef:** "Tek sahnede gerçekten oynanabilir pencere" — arka plan + hareketli
sprite'lar + tıklanabilir hotspot'lar, konsol çıktısı olmadan.

1. **Oynatıcıyı bağla.** `play_main.cpp`: ffmpeg yerine `decode_jpeg_file`, kare
   çizimi için `compose_scene`, hotspot tıklaması mevcut yolu korusun.
   *Oyuncu ne yapar:* sahneyi boyalı arka planıyla ve sprite'larıyla görür,
   hotspot'a tıklayınca arka plan/label değişir.
2. **Çizim geçişini tamamla.** `+0x09 & 0x20` snap geçişi, `0x42b5b2`
   hedef-okuma karışımı, mode-1 kafa→güncel kare dizisi.
   *Oyuncu ne yapar:* aynı sahne özgün görünümüne yaklaşır (vurgu/parlama
   efektleri, kırpma artıkları kaybolur).
3. **Metin/diyalog katmanı.** `0x417a17` + diyalog penceresi + yazı tipi varlığı.
   *Oyuncu ne yapar:* diyalogu **okur** ve yanıt seçer (`0xab`/`0xb0`/`0xb1`
   durumu zaten modelli).

Sonrası: sahneler arası taşınan durum + save/load (1,296 etiketin çözümü) →
oyuncu birden çok sahneyi baştan başlamadan geçebilir.

## Tahminlerim ve henüz denemediklerim

- **[tahmin]** Özgün motor sesi eşzamanlı (async) çalıyor, yalnızca
  diyalog/video oturumlarında bekliyor; bunu yalnızca `0x428cbd`/`0x427a01`
  istek yapılarından çıkarıyorum, davranışı ölçmedim.
- **[tahmin]** `0x42b5b2` karışımı toplamalı (additive) veya sabit-ağırlıklı;
  henüz export'u çözülmedi.
- **[denenmedi]** FFmpeg'in Indeo 3/4'ü tüm 165+ videoda hatasız çözmesi
  (yalnızca iki dosyanın başlığını ve codec adını gördüm).
- **[denenmedi]** macOS/Windows derlemesi ve SDL2 davranışı; bu ortamda yalnızca
  Linux `make` doğrulandı. `cmake` bu makinede kurulu değil, `CMakeLists.txt`
  çalıştırılamadı.
- **[denenmedi]** Metnin bitmap mi TTF mi olduğu (yazı tipi varlıklarına
  bakılmadı).
- **[denenmedi]** Renderer'ı SDL_Surface/Texture'a taşımanın gerçek maliyeti;
  dosya sayısından bağımsız olarak çizim kurallarının yeniden doğrulanmasını
  gerektireceğini düşünüyorum.

## Kısa tablo

| Dosya | Ne yapıyor | Kütüphaneyle değiştirilebilir mi? | Karar |
| --- | --- | --- | --- |
| `port/src/jpeg.cpp`, `include/byzantine/jpeg.hpp` | JPEG çözücü (Huffman/IDCT/upsample/renk) | Evet — libjpeg-turbo | **Koru** (bugün), isteğe bağlı takas |
| `port/src/compose.cpp` | RGB16 yüzey, maskeli blit, PPM | Kısmen (SDL_Surface, PNG) | **Piksel kuralları kalmalı** |
| `port/src/main.cpp` | RLE→TGA yazıcı | Evet (stb_image_write) | Teşhis aracı, dokunma |
| `port/src/rle.cpp` | Oyunun RGB555 RLE'si | Hayır | **Kalmalı** |
| `port/src/anims.cpp` | Kare run-stream + AVI başlığı | Hayır (AVI *başlığı* kalır, *kod çözme* FFmpeg) | **Kalmalı** |
| `port/src/panorama.cpp` | Panorama projeksiyonu, PC53 yuvarlama | Hayır | **Kalmalı** |
| `port/src/cc.cpp` | VM, opcode'lar, RNG, istekler | Hayır | **Kalmalı** |
| `port/src/cc_main.cpp` | CLI, teşhis, `--background/--panorama` | — | Kalmalı |
| `port/src/save.cpp` | Save biçimi (ham RGB16 thumbnail) | Hayır | **Kalmalı** |
| `port/src/play_main.cpp` | SDL2 pencere + **ffmpeg fork** | Evet — iç çözücü + SDL | **Değiştir** (öncelik 1) |
| — | Ses çalma | **miniaudio / SDL2 audio** | Yeni katman |
| — | Video (Indeo 3/4 + PCM) | **FFmpeg libav\*** | Yeni katman |
| — | Metin/yazı tipi | Biçim bilinmiyor; şimdilik özel | Yeni katman |
| `port/Makefile`, `port/CMakeLists.txt` | Derleme/bağımlılıklar | — | Yeni kütüphane satırları burada |

## Kalıcı kullanıcı kararı — 26 Ekim 2026

Kullanıcı bu yaklaşımın sonraki ajanlar tarafından okunup uygulanması için
kalıcı talimat olarak kaydedilmesini istedi. Bağlayıcı proje yönü kökteki
`AGENTS.md` dosyasının "Library strategy and playable milestones" bölümüne,
kısa devam bağlantısı `CONTINUE.md` dosyasına eklendi.

- Standart medya çözücüleri ve ses/pencere/girdi altyapısı yeniden yazılmayacak;
  uygun hazır kütüphaneler kullanılacak. Mevcut SDL2 korunacak.
- Çalışan JPEG çözücüsü şimdilik korunacak; yeni standart codec geliştirmek
  yerine gerektiğinde hazır kütüphane bağlanacak. AVI için FFmpeg gerçek oyun
  dosyalarıyla değerlendirilecek; bütün dosyalar için uyumluluk varsayılmayacak.
- Özel çalışma oyunun veri biçimlerine, kurallarına ve oynanabilir entegrasyona
  odaklanacak. Kaynak motor arayışı kullanıcı yeniden istemedikçe yapılmayacak.
- Ses çıkışı artık SDL2 ile uygulanmış durumda; önceki tablodaki "yeni katman"
  ifadesi tarihsel durumdur. Bu, bütün ses davranışlarının tamamlandığı anlamına
  gelmez. Oturumlar, zamanlama, karıştırma/döngü/parametreler ayrıca doğrulanmalı.
- Son incelemede `make -C port -j2 all test` geçti. VT 72.btb / 497 ve tıklama
  14 sanal SDL aygıtlarıyla çalıştırıldı: beklemeler 4. ve 1. adımda, son tıklama
  12 adım; 759 çizim, 19 olay, bir yok sayılan tıklama ve H değişimi gözlendi.
  Gerçek hoparlör/pencere ve özgün motor eşitliği bu testle doğrulanmış değildir.

Bu kayıt yalnızca dokümantasyon değişikliğidir; yeni binary davranışı türetilmedi,
çalışma zamanı kodu değiştirilmedi.
