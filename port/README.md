# Byzantine taşınabilir kaynak kodu: ilk parça

Güncel kısa devam kaydı: [`../CONTINUE.md`](../CONTINUE.md).

## Disklere göre animasyon seçimi

Launcher kayıt dosyasındaki disk numarasını okur ve o diskin animasyonlarını
ayrı bir `disc-media/` önbelleğinde tutar. Aynı adlı dosyaların farklı disklerde
farklı içerikleri olabilir; seçilen diskin dosyaları ortak çıkarılmış klasöre
önceliklidir. İlk açılışta bir kez ek çıkarma yapılır. Doğrudan CLI kullanırken
`--anims-overlay <disk-klasoru>` aynı önceliği sağlar. Oturum içindeki disk
isteme komutları henüz otomatik disk değiştirme uygulamaz.

## Her açılış için otomatik log

Normal `./port/scene.sh ...` komutu, hazırlık aşaması dahil her denemeyi
`port/output/logs/` altında ayrı dosyalara kaydeder. Terminalde dosya yolları
basılır; oynama komutunu değiştirmen gerekmez.

- `run-....log`: zaman damgalı standart çıktılar, hatalar ve çıkış durumu.
- `run-....trace.log`: sıralı SDL fare/tuş/pencere olayları, betik komutları,
  değişken yazımları, konuşma komutları ve durum görüntüleri. Fare hareketleri
  konsolu doldurmaz; bu dosyaya gider.
- `run-....summary.json`: başlatma komutu, Git sürümü/değişmiş dosyalar,
  çalıştırılan oyuncunun SHA-256 değeri, cihaz ortamı, süre, çıkış durumu,
  olay sayıları ve son önemli satırlar.
- `LATEST`: son başlatılan denemenin `.log` dosyasının yolu. Önce bununla
  eşleşen `.summary.json` okunmalı; ardından gereken log bölümü aranmalı.

`BYZ_LOG_DIR=/istenen/klasor ./port/scene.sh ...` farklı klasör seçer. Eski
loglar otomatik silinmez. Aynı anda açılan denemelerin dosyaları ayrıdır.
Doğrudan ikiliyi çalıştırırken aynı kayıt için
`python3 port/run_logged.py -- ./port/byzantine-play ...` kullanılabilir.

Betik izleri deneme hesaplamalarını ve ses kapıları için tekrar yürütmeleri de
kapsar: `semantics=evaluation_not_commit`. İşlemin uygulanıp uygulanmadığını
`click_result`, hata/geri alma satırları ve `state_adopted` ile birlikte değerlendir.
Bu kayıtlar oyun kaydı veya otomatik tekrar oynatma dosyası değildir. Her çizim
karesinin piksel verisini, kaçırılmış fiziksel girişleri veya hoparlör çıkışını
kaydetmez. Normal çıkış ve yönlendirilen SIGTERM/SIGINT özete yazılır; kayıt
süreci SIGKILL ile öldürülürse özet `running` kalabilir, yazılmış log korunur.

Panorama kamera tüketimi ve 16 bit kenar sarma tamponu, bağımsız C++17
yardımcıları olarak `src/panorama.cpp` içine eklendi ve `panorama-test` ile
sınandı. Devam adımında açı dönüşümü, yaw piksel ofseti, pitch sınırı ve açık
boyutlardan kenar tamponu hesabı eklendi. Hesaplar özgün sabitleri kullanır;
host libm/x87 sonuçlarının bit eşitliği henüz doğrulanmadı. Yeni kanıt:
[`PANORAMA_PROJECTION.md`](../analysis/rea/PANORAMA_PROJECTION.md).
Son adımda kosinüs/arktanjant tabloları ve koşullu nearest/even koordinat
nicemlemesi eklendi. Yazılmamış tablo hücrelerine erişim reddedilir. CRT başlangıcı
53 bit x87 hassasiyeti seçiyor; etkin yuvarlama modu ve özgün CRT tablo değerleri
henüz çalışma zamanı ile karşılaştırılmadı. Kanıt:
[`PANORAMA_TABLES.md`](../analysis/rea/PANORAMA_TABLES.md).
Son eşleme adımında `panorama_mapping`, açık kaynak/viewport boyutları,
kenar dolgusu, pitch ve zoom ile RGB16 kaynak piksel ofsetlerini üretir.
Ofsetler yaw eklenmeden önceki sarılmış tampon içindir. İşlemler ayrı binary64
adımlar kullanır; host nearest yuvarlama gerekir. Geçersiz derinlik, tablo ve
kaynak sınırları açıkça reddedilir. Küçük elle doğrulanabilir eşlemeler ve üç
640×480 sentetik görünüm sınandı. Kanıt:
[`PANORAMA_MAPPING.md`](../analysis/rea/PANORAMA_MAPPING.md).
Normal RGB16 örnekleme dalı `sample_panorama_rgb16` ile eklendi: açık yaw
ofseti, hedef dikdörtgen ve pozitif çift bayt satır adımıyla 16 bit sözcükleri
korur. Son kaynak adresleri çizimden önce denetlenir; sıfır dışı efektler ve
desteklenmeyen bellek düzenleri açıkça reddedilir. Kanıt ve eşleme yenileme
koşulları: [`PANORAMA_SAMPLING.md`](../analysis/rea/PANORAMA_SAMPLING.md).
Özgün x87/CRT ile piksel eşitliği henüz doğrulanmadı; kamera/eşleme önbelleği
ve `0xb9` bağlantısı henüz uygulanmadı.

Son adımda yükleme-time durumu eklendi: `PanoramaProjection::set_source`
`0x4236b5` yolunu modeller — ölçek/kenar/satır adımı yalnızca çözülen boyutlar
değiştiğinde yeniden hesaplanır, kamera sıfırlaması ve eşleme yeniden kurulumu
ise her başarılı görsel yüklemesinde çalışır. `panorama_geometry` artık
`stride_words` (= width + 2*padding) alanını da verir. Aynı adımda yaw ofseti
binary64 (PC53) çarpıma çevrildi; önceki `long double` genişletmesi
`64/(2*pi)*pi` için 32 yerine 31 üretiyordu. Kanıt:
[`PANORAMA_PRECISION_FIX.md`](../analysis/rea/PANORAMA_PRECISION_FIX.md) ve
[`PANORAMA_LOAD_STATE.md`](../analysis/rea/PANORAMA_LOAD_STATE.md).
Kanıt ve kalan sınırlar: [`PANORAMA_CAMERA.md`](../analysis/rea/PANORAMA_CAMERA.md).
Panorama devam adımında `0xba` komutu, sonraki yükleme için yatay konumu
0–2495 aralığında `SceneState` içinde saklar. `--panoramas <CO kaydı>`,
`0xb9` düğüm numaralarını özgün CO.ALL adlarına ve JPEG yollarına bağlar.
Bu statik eşleme panorama çizimi değildir; `0xb9` hâlâ açıkça durur.
44 sahnede 182 yükleme komutu incelendi, 165'i ISO JPEG yollarıyla eşleşti.
Kanıt ve testler: [`PANORAMA_BINDINGS.md`](../analysis/rea/PANORAMA_BINDINGS.md).

```sh
port/byzantine-cc port/output/cc_all/EA --panoramas port/output/co_all/EA
```

Bu dizin, özgün kaynak kodun kopyası değildir. `BETRAYAL.EXE` üzerinde REA/Ghidra ile gözlenen davranıştan yeniden yazılan C++17 kodudur. Hedef modern Windows, Debian ve macOS için ortak bir oyun motoru kurmak.

Önceki adımda (25 Ekim 2026) `savegame/NN.BTB` biçimi belgelendi ve test edilmiş
bir ayrıştırıcı eklendi. Son adımda aynı görüntü `load_save` ile
`SceneState`'e geri yüklendi ve `0xcc`/`0x70` gerçek dallara dönüştü (aşağıda).
CD1 ISO'sundaki 77 kayıt (74 numaralı + autosave + tempsave + `NEW.CAN`)
çökmeden ayrıştırılıyor, her nesne kaydının `+0x0c` etiketi kendi sahnesinin CC
etiket kümesinde bulunuyor. Kanıt ve bilinmeyen listesi:
[`SAVE_FORMAT.md`](../analysis/rea/SAVE_FORMAT.md) ve
[`SAVE_RESTORE.md`](../analysis/rea/SAVE_RESTORE.md).

## Şu anda çalışan parçalar

- `byzantine-rle`: Oyunun RGB555 RLE animasyon/görsellerini RGBA olarak çözer ve saydam TGA kareleri yazar. Çekirdek kod `include/byzantine/rle.hpp` ve `src/rle.cpp` içindedir. REA kanıtları `analysis/rea/README.md` içinde.
- `byzantine-all`: Oyunun `engine/*.ALL` arşivlerini listeler veya içindeki kayıtları çıkarır. REA, `FUN_00411b4b` işlevinin arşiv başındaki sayıyı, sıfır sonlandırmalı adları ve iki adet 32 bit ofseti okuduğunu gösterdi (`ev_3ebfbe148234712a019aa691f8cdf3501d5f1b2d905f1ac7098de35acf7077a9`). `Byz.all` üzerinde ikinci ofsetin uzunluk değil bitiş ofseti olduğu, dizin sonu ve dosya uzunluğu ile doğrulandı.
- `byzantine-cc`: `CC.ALL` içindeki derlenmiş sahne komutlarını ayrıştırır. `FUN_00412762` okuma işlevi, 0x32–0xE1 işlem kodu aralığı, işlem kodu başına argüman sayısı tablosu ve argüman türüne göre 0/1/2/4 baytlık veriler REA ile gözlendi (`ev_961516329fda54629fb59b3d818cb49b28be3122a552ef037259a378fdc01a23`, `ev_0c452b2e3ab8886e9627eec37b95ff26707255cc17cf2aa7718d5ad766e61e32`). Komutların oyun içindeki anlamı henüz uygulanmadı.
- `byzantine-play`: SDL2 sahne oynatıcısı (teslimat: çalışan sahne). Gerçek bir
  savegame'in durumunu (`objects`/`media`/`sounds`/`records`, taşınan 3000
  sözcüklü değişken havuzu) `byzantine-cc --trace-save` ile **aynı** yükleyiciden
  alır (`src/scene_setup.cpp`), `0x41045b`'nin arka plan JPEG'ini gömülü
  `decode_jpeg` ile açar (ffmpeg/dış süreç yok), kayıtları `compose_scene`
  (0x43b216) ile birleştirir ve 640×400 kuşağı pencereye çizer. Tıklama `0x72`
  alanlarını **640×480 yüzey** koordinatında sınar (`y - 0x14`), `action`
  etiketini çalıştırır; iz `unresolved_*`/`unsupported_opcode` ile durmazsa yeni
  durum çizilir (kayıt türleri ve etkin alanlar değişir). Ses: SDL2 audio cihazı
  + WAV (PCM16); bir `0xd1`/`0xd3` isteği **betiği** durdurur, pencereyi değil:
  ses çalarken olay döngüsü ve çizim sürer (`H` çalışır, `Esc` çıkar), tıklamalar
  yok sayılır; ses bitince betik sesin sırasında kaldığı yerden devam eder.
  Tıklamayla başlayan etiketler de aynı yolu kullanır (önce betik tamamlanıp
  sonra ses çalınmaz). `H` alan çerçevelerini açar/kapatır, `Esc` çıkar.
  `0xbd`/konuşma videoları oynarken kareler çerçeveler **çizilmeden** sunulur:
  çerçeve çizimi portun tanı/sürüm kolaylığıdır, özgün yüzeyde yoktur ve
  videonun üstünde kalmamalıdır (harita işaretlerinin geçiş videosunda kalması
  bu yüzden düzeltildi).
  `--selftest-events` bu bekleme sırasında `H` ve bir tıklama enjekte edip
  sayaçları basar (başsız doğrulama).
  **Panorama**: bir `0xb9` düğümü yüklendiğinde sahne katmanı
  `nodes\<2h>\<ad>.jpg`'dir; `PanoramaView` (`src/panorama_view.cpp`) onu
  `0x4236b5` dikiş satırlarıyla genişletir, `0x401ddf`'nin sahnelenmiş piksel
  konumunu kamera deltasına çevirir ve `0x4242bd` gibi örnekler. Bakış `←`/`→`
  (10°) ve `↑`/`↓` (5°) ile ya da panaromayı fareyle sürükleyerek döner
  (sürükleme tıklama sayılmaz). Tıklama artık `0x401ca6`/`0x4240ae`'nin ters
  eşlemesiyle **kaynak** koordinatına çevrilir; bu yüzden x > 640 olan panorama
  alanları da tıklanabilir. Kayıtlar (sprite'lar) da panorama sahnesinde
  **kaynak** koordinatındadır: `0x40241c`'nin panorama dalı `0x423e03` ile
  dikdörtgenin köşelerini ekrana izdüşürür ve ilk görünen köşe sprite'ı
  yerleştirir; bu yüzden SM'deki İma (`Smim00b`, kaynak 1302,333) kamera ona
  döndüğünde görünür. `--staged-x 0..2495` başsız bir bakış açar,
  `--panorama-yaw [SAHNE:]derece` (tekrarlanabilir; önek yoksa her panorama
  sahnesinde) her karede mutlak bir kamera yaw'ı uygular (tanılama girdisi),
  `--click-at <x>,<y>` ekran noktasını gerçek tıklama yolundan geçirir.
  `--cursor-at <x>,<y>` aynı nokta için oyunun seçtiği imleci kurup yazar
  (başsız imleç tanılaması); imleç tabloları ve sınırlar: `CURSORS.md`.
  `0xcb` dönüşü `0x402285` gibi adım adım uygulanır.
  **Sahneler arası geçiş**: bir tıklama etiketi `0x73`'e ulaşırsa `DAT_00482428`
  hedefi çözülür, per-scene tablolar (nesneler/sesler/diyalog zincirleri/medya/
  kayıtlar) boşaltılır ve hedef sahne `0x42c8a0` gibi **ilk komutundan** yüklenir;
  taşınan tek durum `DAT_0047e2f0` 3000 sözcüklü değişken havuzudur (ayrıntı:
  [`SCENE_TRAVEL.md`](../analysis/rea/SCENE_TRAVEL.md)). Yeni sahnenin girişi de
  ses kapılıdır; dönüşte konsola `travel …` ve `entry …` satırları basılır.
  Girişin başında bir `0x70` (diyalog) varsa **o da çalıştırılır** ve sonucu
  betiği sürdürür (`entry_conversation …`); MR'a varıştaki `MMhmr1a` gibi bir
  karşılama diyaloğu atlanırsa `0xb8`'in askıya aldığı tıklama alanları geri
  gelmez ve sahne "çıkışsız" kalır. Tıklanabilir hiç alan yoksa artık
  `entry_no_hotspots`/`restart_no_hotspots` satırı basılır
  (`analysis/rea/SCENE_ENTRY_CONVERSATION.md`).
  **Metin pencereleri**: `0x4177c7` (inceleme, font 1, x=5, y=0x14a, 3 satır) ve
  `0x417a17` (mesaj, font 3, sağa dayalı 0x26e/0x2d) artık çizilir; içerik
  `engine/<scene>ce` (`CE.ALL`, gerçek Türkçe metin), font
  `klio.SFN`+`GR_KLIO.TGA`/`BK_KLIO.TGA` ve `0x4067e2` karışımıdır (`--ce`,
  `--fonts`). Bir nesneye tıklamak `+0x14` metin indeksini gösterir ve
  nesnenin `action` etiketi betiği sürdürür. Not (düzeltme 26 Ekim 2026):
  `0x70` konuşmalarının altyazısı **vardır**; `engine/<stem>.byz` (`Byz.all`
  içinde paketli) satır satır metni, `.ccf` ses klibi adlarını verir ve
  `0x424780` metni çizer, `0x40d2a1` eşleşen `dialog/<ad>/<klip>.avi`'yi
  oynatır. Ayrıntı ve MR örneği: [`CONVERSATION.md`](../analysis/rea/CONVERSATION.md).
  Kapsam dışı: mesaj paneli
  arkası, saniyede bir satır açılan kaydırma (`0x4177c7`/`0x4176d8`) ve
  `0x4069a4`'ün SFN atlas çizimi. Video (`0xbd`) ve geçiş efektleri yoktur.
  **Animasyon hızı**: `0x43b216` (çiz + `0x43bb77` ile kare ilerlet) orijinalde
  sınırsız döngüde çalışır; `+0x04` gecikmeleri bu pass hızının bölenleridir.
  Port 16 ms'lik çizim tikiyle çizer ama kayıt-pass saatini ayrı tutar
  (`--anim-pass-hz`, varsayılan 250; 20 gecikmeli bir kayıt ~80 ms'de bir kare
  ilerler). Detay: [`ANIMATION_FRAME_PACING.md`](../analysis/rea/ANIMATION_FRAME_PACING.md).
  **İmleçler (oyunun kendi fare imleçleri)**: `0x40dd7f`/`0x40dae5` iki ayrı
  32'lik tabloyu `cursors/<ad>*.tga` (normal) ve `cursors/klio/<ad>*.tga`
  (alternatif mod, `0x68` -> `DAT_00464878`) olarak yükler; adlar ve hotspot'lar
  EXE'de sabittir (`0x45dbe8`, `0x45e090`). Hangi imlecin kurulacağını
  `0x40c329` belirler: sahne alanında vurgulanan nesnenin `+0x09` **tür baytı**
  imleç kimliğidir (0x0a talk, 0x0b exit, 0x0c magnify, 0x0d take, 0x0e manplte,
  0x12–0x14 eşya kullanımı, 0x18–0x1b panorama okları, 0x1c hotbig, 0x1e
  hotsmall ...). Elde eşya varken `0x407bd7` `inventor/<ad>.tga`'yı, kullanım
  alanı (`0x12/0x13/0x14`) üstünde `inventor/hotitems/<h-ad>.tga`'yı seçer.
  `0x4083d4` envanter bandını (slotlar, sayfa okları, inceleme düğmesi, kanal
  alanı) aynı kuralla işler; `0x40de7b` normal sette ve panorama dışında
  `0`/`0x17` -> `0x1d` ("map"), `0x1c` -> `0x1e` ("hotsmall") küçük varyantına
  çevirir. Port bunları `SDL_CreateColorCursor` ile gerçek imleç olarak kurar
  (`--cursors`, varsayılan `<backgrounds>/cursors`); eksik sanat `cursor_missing`
  + sistem oku olarak bildirilir. `0x40ebe1`'in 250 ms'lik çok kareli imleç
  animasyonu ve `0x4013ae`'nin kenar-bandı okları henüz uygulanmıyor. Ayrıntı,
  tablolar ve doğrulama: [`CURSORS.md`](../analysis/rea/CURSORS.md).
- **Konuşmalar (`0x70`)**: seçilen örnek MR etiketi 413, konuşma kaydı 2 =
  `IMhMR2a`. `0x70` operand 0'ı nesne değil `engine/<scene>cd` **kayıt indeksi**
  olarak okur; VM `0x70`'te durur, gerçek `.ccb` komutlarını yürütür, `.byz`
  altyazısını ve cevap listesini çizer, `.ccf`/`dialog/<ad>/<klip>.avi` medyasını
  oynatır. Ekran **640×480**: oyun alanı `0x14..0x413`, alt yazı bandı
  `0x1a4..0x1df`. Altyazı `0x424981`'in verdiği **x=55, y=420**, 530 px sarma,
  3 satır ve yeşil **0x03eb** ile çizilir; cevaplar `0x436b01`'in verdiği
  **x=100**, 440 px sarma, `0x7f` yıldızı, turuncu **0xe2c0** / fare üstünde
  beyaz **0xffff** ve alt sınır **370** ile video üzerine gelir. Tıklama
  dikdörtgeni çizilen satır bloğudur. Türkçe `CAPTIONS.SFN`'de 9 harf boş
  olduğu için aynı glifleri taşıyan `DIALOG.SFN` kullanılır ve bu durum açıkça
  bildirilir; konum/sarma/renk orijinal koddan gelir. Metin yeni konuşmada
  temizlenir, cevaplar açılırken korunur, seçilen cevap altyazı olur ve çıkışta
  silinir. Konuşma `0x39`/cevap sonunda `0xffff` dönerse betik `0x70`'ten
  sonraki komutta sürer (bu örnekte `0x73` ile **MAP**'e gider); bir sahne
  nesnesine tıklanırsa o nesnenin `+0x0c` etiketi çalışır. Değişken yazımları
  `SceneState` üzerinden taşınır; yarım kalan konuşma commit edilmez.
  İndeo 4 videosu FFmpeg ile bir kez JPEG+PCM önbelleğine çözülür; oynatma
  **640×400 tam oyun alanına 2×** (kaynak 320×200, `0x43a248` mod 2) ve PCM
  AVI içinden SDL2'ye verilir. Türkçe metin `0x405bf4`'ün `byte - 0x41` font
  indeksiyle çizilir (CP1254 dönüşümü yok); ses aygıtı/klip hatası açıkça
  bildirilir, sessizce yutulmaz. Konuşma normal oynanışta kayıttan başlatılır:
  `30.btb` MR etiketi 0 → kapı alanı 42 → mantıksal `(300,300)` (panorama alanı
  1, eylem 261) → `0x70` kayıt 2. Aynı sahnedeki yeniden konuşma
  `DAT_00482ad0` durumunu korur (sorulmuş/seçilmiş cevaplar, devre dışı
  girişler, varsayılan grup); `0x73` sahne yüklemesi bu durumu serbest bırakır.
  Cevap listesinde ESC `0xfffe` ile konuşmayı iptal eder ve oyuncuya döner
  (tüm oyuncuyu kapatmaz). `--conversation-cancel <n>` ve
  `--print-hotspots` tanılama içindir. Deneme:
  `./port/scene.sh . MR 0 30.btb --entry`; kapı `(352,201)`, satıcı
  `(300,300)`, seçenekler y≈250/270/310, altyazı 420; konuşma sonrası
  Left/Right ile panorama döndürülüp alan 17'ye tıklanır. Ayrıntı, opcode
  tablosu, kalıcılık kanıtı ve engeller:
  [`CONVERSATION.md`](../analysis/rea/CONVERSATION.md).
- `byzantine-cc --hotspots`, işlem kodu `0x72` olan sabit alan tanımlarını çıkarır. Oyunun `FUN_0042db22` komut yürütücüsü bu tanımları tıklama alanı tablosuna yazıyor: alan numarası, x, y+20, genişlik, yükseklik ve kalan iki değer. Yedinci değerin alt baytı alan durumuna yazılıyor; diğer değerlerin tüm anlamları henüz çözülmedi (`ev_6fae0653eb9d17a560b88cf841ffbe1c2fc3b14d9c5f18ac7a7ef5e9853ca522`). Dinamik operandlı tanımlar atlanır; çıktı betik yürütme sırasını göstermez.
- `byzantine-cc --backgrounds <CB kaydı>`, sabit `0x69` sahne komutu numaralarını aynı sahnenin `CB.ALL` ad listesiyle eşler. `CB.ALL` kayıtları `ad\0kaynak\0ff` dizilerinden oluşur. Dinamik numaralar ancak çalışan yorumlayıcıda çözülebilir.
- `byzantine-cc --flow`, `0xe1` etiketleri ile `0x76` atlamalarını ve `0x9b` çağrılarını eşler. Oyunun `FUN_0042d383` işlevi etiketi sonraki komuta bağlar; aynı etiket birden çok kez tanımlanırsa son tanım geçerlidir. `FUN_0043194f` çağrı dönüş adresini saklar; `FUN_0042d8ba` dönüş komutları `0x6f` ve `0x83` için yığını boşaltır. Bu çıktı koşullu dalları çalıştırmaz.
- `byzantine-cc --labels`, çözülmüş etiketleri ve her etiketin başladığı komut indeksini verir. Bu, betik yollarını tek tek, ham `CC.ALL` baytlarını yeniden ayrıştırmadan izlemek içindir.
- `byzantine-cc --trace <etiket> <CB kaydı>`, doğrulanan küçük komut kümesini tek etiketten yürütür: arka plan yükleme, alan açma/kapama, atlama, çağrı ve dönüş. Başka bir komuta gelirse durup işlem kodunu bildirir. Bu, tam oyun yorumlayıcısı değildir; alan tanımları betikten önceden okunur.
- İzleyici ayrıca `0x67` ile sahne-içi değişken atamasını, `0x71` eşitlik ve `0x77` eşitsizlik koşullu dallarını yürütür. Bu üç komut EXE'deki `FUN_0042db22` ve operand çözücüsü `FUN_004319b3` üzerinden doğrulandı. Tek etiket izinde önceden kurulmuş bir değişken gerekirse araç değer uydurmaz; `unresolved_variable=<id>` ile durur. Sahne dışından taşınan durum henüz uygulanmamıştır.
- Oyuncu aynı CC betiğindeki tıklamalar arasında `SceneState` içindeki değişken
  değerlerini ve etkin alanları korur. Bağımsız `--trace` çağrıları temiz durumla
  başlar. `make -C port test`, iki etiket arasında atanan değerin koşullu dalda
  kullanılmasını ve gerçek EA 475 → 595 geçişini sınar. EA'daki alan 94'ün
  dönüş yolunda gereken 5 numaralı panorama değişkeni henüz kurulmadığından
  tam gidiş-dönüş desteklenmez. Oyuncu eksik değişken veya desteklenmeyen komut
  halinde nedeni pencere başlığına ve terminale yazar; o tıklamanın kısmi
  durum değişikliklerini uygulamaz.
- `0x9d` eşitlik ve `0x9e` eşitsizlik koşullu çağrılarını yürütür. EXE'deki
  `FUN_0042db22` bu koşullarda `FUN_0043194f` ile dönüş adresini saklar.
  Testler her iki komutun doğru/yanlış koşullarını ve çağrı sonrası devamını
  doğrular.
- `0x96`–`0x99` sıralama koşullu atlamaları ve `0x9f`–`0xa2` koşullu çağrıları yürütür. Karşılaştırmalar işaretsiz 16 bit değerlerle yapılır; bilinmeyen değişkenlerde iz durur. 144 karşılaştırma, 96 eksik durum kontrolü ve özgün EA etiketleriyle doğrulandı. Kanıt ve kapsam: `analysis/rea/ORDERED_COMPARISONS.md`.
- `0x74` değişkene ekler, `0x75` değişkenden çıkarır. Operand çözümü
  `FUN_004319b3`, işlemler `FUN_0042db22` dallarından alındı. Toplama sonucu
  16 bite sarılır; çıkarma negatif sonuç verecekse özgün hata dalına karşılık
  açık hata üretilir. Önceki değeri bilinmeyen değişkene işlem uygulanmaz.
- `0x90` kaynak yenileme/uyumlama isteği olarak izlenir. Özgün kod, seçici sıfırken etkin girişleri kapatır (`FUN_00432303`), tüm yüklü kaynakları uzlaştırır (`FUN_00432401`) ve yenileme bayrağını kurar. Taşınabilir çizici henüz olmadığı için izleyici bunu `resource_refresh_requests` olarak raporlar; kaynakları varmış gibi davranmaz.
- `0xb8`, etkin alanları sahne durumundaki geçiş tablosuna kaydedip kapatır (`FUN_00438cf9`). `0xb7`, kaydedilmiş alanları yeniden açıp kaydı tüketir (`FUN_00438d78`); bu arada açılan diğer alanlar korunur. İkinci `0xb8` önceki kaydı değiştirir. Ayrıntı ve doğrulama: `analysis/rea/HOTSPOT_TRANSITIONS.md`.
- Diyalog durumu `SceneState::dialogue_records` içinde tutulur: `0x408662` her
  kaydı ilk baytı `0` olacak şekilde ayırır, `0xab` satırı başlatır/durdurur ve
  mod baytını yazar, `0xb0`/`0xb1` bu bayta göre gerçek dallanma yapar, `0xb6`
  satırı öne çekip eski state-3 satırlarını `1`e döndürür. Kayıt kimlikleri
  global `engine/SHOW.LCI` sırasıdır (CD1'de 126 kayıt); betik operandı önce
  sahnenin `CI`/`CY` tablosundan `0x408bb2`/`0x408c81` ile bu sıraya çevrilir.
  Bu dört tablo `--lci`, `--ci`, `--lcy`, `--cy` ile verilir. `0x4077b2`'nin
  kanal yığınına ekleme kararı `0x407254`'ün `inventor/<ad>.tga` +
  `inventor/hotitems/<h-ad>.tga` yüklemesine bağlıdır; bu nedenle medya adları
  `--inventor`, `--hotitems` ile verilir, verilmezse iz `unresolved_dialogue_inventory`
  ile durur. Tablolar yoksa `unresolved_dialogue`/`unresolved_dialogue_channel`
  bildirilir; çizim (`0x407420`/`0x407b9c`) yalnızca istek olarak sayılır.
  Kanıt ve ölçümler: `analysis/rea/DIALOGUE_RECORDS.md`.
- `0xb2`/`0xb3` aynı diyalog karşılaştırmasının çağrı biçimidir; `0x43194f`
  dönüş adresi izleyicinin çağrı yığınına yazılır. `0xbf`/`0xc0`/`0xc2`,
  `DAT_00486220` ses parametrelerini (`+0x1c/+0x20/+0x24/+0x28/+0x2c`) çözümlenmiş
  operandlardan yazar; operand 3 yalnızca türü 2 ise kullanılır. Ses aygıtına
  aktaran `0x427d1a` çalınmaz, `sound_refresh_requests` olarak sayılır.
- `0xbd` video komutudur: operand 0 sahnenin `engine/<scene>cf` kaydını  (`DAT_00484330`), operand 1 kipi seçer (0 slide, 1 intro, 2 site, 3 transit,
  4 char, 5 anim), operand 2/3 yalnızca türü `0x40` ise okunur. Birincil yol
  `avi/<ad>.avi`, yedek `video\<kip>\<ad>.avi`. 2 ve 3 kipleri yalnızca
  `gamepref[5]`/`gamepref[4]` sıfırdan farklıysa oynar (`0x41d440`; dosya
  `savegame/gamepref_%03d`, `0x41f753`). 0/2/4 kipleri ayrıca `0xb9`'un tükettiği
  `DAT_00460664` değerini yazar. Çalma artık gerçektir: klip FFmpeg ile bir kez
  JPEG+PCM önbelleğine çözülür (`FFmpeg`, yeni kodek yok) ve `0x413bdd` gibi
  **betiği bloklayarak** oynatılır; konum/ölçek `0x40d3af`/`0x43a248` kurallarıdır
  (kip 1/2/4 `gamepref[1]` ile 2x 640x400 ya da 320x200 pencere, kip 0/3 2x,
  kip 5/varsayılan 1:1). Klip oynadıysa `DAT_004859cd = 1` uygulanır (kip 5 ve
  `mcaft1a1` hariç). Eksik klip/`--no-video` sessizce yutulmaz:
  `video_request`, `video_media`, `video skipped reason=...`, `video` ve
  `video_end` satırları yazılır. `scene.sh` klibi kullanıcının ISO'larından
  yalnızca gerektiğinde, kaydetin diski önce olacak şekilde çıkarır. Tablo
  verilmezse `unresolved_video_name`, oyun tercihleri verilmezse
  `unresolved_video_setting` ile durulur. Kanıt:
  `analysis/rea/VIDEO_AND_AUDIO.md`, `analysis/rea/CF_VIDEO_PLAYBACK.md`.
- `0xb9` panorama/düğüm yükleyicisidir: ham operand 0 CO.ALL kaydını
  (`DAT_00483d90`) seçer, ad `nodes/<ilk 2>/<ad>.jpg` yolunu kurar, operand 1/2
  değişken tablosundan çözülür, operand 3 geçiş kimliğidir (`0x432133`: 1→0x67,
  2→0x65, 3→0x66, diğer→100). Operand 3 değişken değilse `0xbd`'nin yazdığı
  `DAT_00460664` geçiş kodu tüketilir. `0xba`'nın sakladığı yatay konum
  `0x401ddf` gibi tüketilir; `ScenePanoramaLoad` kayıt/ad/yol/konum/geçiş
  bilgisini tutar. CO.ALL `--co <kayıt>` ile verilir; verilmezse iz
  `unresolved_panorama_node` ile durur. JPEG çözme, projeksiyon ve geçiş
  karışımları kapsam dışıdır. Kanıt: `analysis/rea/PANORAMA_LOAD.md`.
- `0xcc` ve `0x70` artık nesne tablosu üzerinde gerçek dallardır. `0xcc`,
  `DAT_00464300[ham(0)]+0x14 = ham(1)` yazar; iki operand da `0x436652` ile ham
  sözcük olarak okunur, değişken çözümü (`0x4319b3`) uygulanmaz. Özgün kodda NULL
  denetimi yoktur; eksik yuva açık `missing_object` sonucudur, asla örtük
  ayırma değildir. `0x70` CONVERSE, nesnenin `+0x0c` etiketini
  `DAT_004654e0`/`resolve_control_flow` üzerinden CC komutuna çevirip `pc`'yi
  değiştirir. Özgün iç diyalog koşucusu `0x432d6e` yeniden kurulmadığı için
  port, operand 0'ı o koşucunun döndürdüğü nesne kimliği kabul eder; bu, koşucu
  o nesneyi döndürdüğünde özgünün yaptığı etiket aramasının aynısıdır.
  `--trace-save <kayıt> <etiket> <CB>` gerçek kayıttan nesne/medya/ses/düğüm
  tablolarını yükleyip izler; `--save <kayıt>` normal `--trace`/`--census` için
  aynısını yapar. Kanıt: `analysis/rea/SAVE_RESTORE.md`.
- Kayıttaki taşınan değişken havuzu `--engine <dizin>` ile geri yüklenir:
  `0x42c8a0`'ın 50×300 kimlik eşlemesini `0x438173`'in LCV global kimlikleriyle
  birleştiren `0x46ee00` tablosu üzerinden `DAT_0047e2f0` izdüşürülür
  (`load_save_variables`). `SceneState::variables` ham yerel operand anahtarını
  korur; EA yerel 5 → global 8 (`lastNode`), 26 → 74 (`flowChk`), 133 → kimlik.
  `load_save` ayrıca `0x408adc`'nin diyalog penceresini geri yükler: kanal
  seçicisi, kaydırma, on kanalın derinlikleri ve 350'lik yığınları (en üst
  önce), geçerli kayıt ve `0x4089b0` kayıt durum baytları. Kayıt kapasitesi
  dosyadaki çift sayısıdır (126), sabit değil; 350'yi aşan derinlik ve kaydı
  olmayan yığın kimliği reddedilir. `--engine` verilmezse değişken havuzu
  bilinmeyen kalır. `0x75` negatif çıkarma özgün `0x40ac43`/`0x40ac38` gibi
  günlüklenip yazılmadan sürer; artık `subtraction_underflows` sayacıdır.
  Kanıt: `analysis/rea/SAVE_VARIABLES.md`.
- `0x6a` medya tanımı (`DAT_00464f60`): dört operand — ham yuva, çözümlenmiş
  `+0x00`, ham `+0x0c` ve türü 2 ise operand 3'ün **alt baytı** `+0x11`;
  `+0x10=1`, `+0x04=0`. `timeGetTime()` yeniden üretilemez, `+0x08=0` ve
  `media_timestamps_unavailable` sayılır. `0xd1` `0x428cbd` çağrısını
  `SceneSoundStart` isteği olarak tutar (id, mode, text, kayıt ve
  `+0x18`/gamepref[2] kapısı); ses aygıtı açılmaz. `0xb4`/`0xb5`
  `dialogue_visible` alanını 0/1 yapar (kayıttan `field_45d1c0` geri yüklenir).
  `0xbb` `0x408c81` ile CY kanalını seçip kaydırmayı sıfırlar. `0x73`
  `engine/<bölüm>.lcx`'ten dolan 50 girişlik `DAT_00482460` tablosunu
  indeksleyip `DAT_00482428` hedefini `scene_switch_requested` olarak bildirir
  ve eski betikte devam etmez; LCX `--lcx <SHOW.LCX>` ile verilir. Kanıt:
  `analysis/rea/SOUND_MEDIA_STATE.md`.
- Kayıt bloğu 24 (`DAT_00486a10`) diyalog zincirleri artık
  `SceneState::dialogue_chains` içine geri yükleniyor (25 kimlik + `+0x64`
  sayacı). `0xc6` çözümlenmiş CI adlarını ekler, `0xc7` bir kimliği silip
  zinciri toparlar; olmayan zincir `missing_dialogue_chain` olur. `0x80` medya
  `+0x0c` yazar. `0x8a`/`0x8b` düğüm ek liste/`+0x2c` komutlarıdır; kayıtta
  olmayan düğüm `unresolved_node_asset` ile durur. `0xbe`/`0xd2`
  kayıt/autosave **istekleridir** (examine sahnesi + `gamepref[0]==1` kapısı;
  dosya yazılmaz). `0xc8`, çözümlenmiş operand 0 ile seçilen sistem komutudur:
  `0x1e` zaman kaynaklı RNG yeniden tohumu (`unresolved_random`), `0x1f` girdi
  üçlüsü (`unresolved_system_input`), `0x20` girdi bayrağı, `0x32`/`0x33`
  `field_460668`, `0x36..0x3a` `status.000` isteği. `0xcb` panorama döndürme
  isteği, `0xd4`/`0xd5` `DAT_004859cd` 1/0. Kanıt:
  `analysis/rea/CHAIN_SAVE_STATE.md`.
- Sahne kompozisyonu eklendi: `0x43b216`'nın çizim döngüsü (tür 0..4, kimlikler
  yüksekten alçağa) `0x43c114` dikdörtgeni -> `0x40241c` 640x400 kırpması ->
  `0x42b679` satır kopyası ile çizer; kopya saydamlık taşımadığı için sprite'ı
  `0x40bbfd` doğrudan hedefe yazar. Port `compose_scene` ile kayıtların güncel
  karesini kapsama maskesiyle yüzeye çizer ve `--compose-out <dosya.ppm>`
  (`--surface <WxH>`, varsayılan 640x400) ile PPM yazar; `composed …` satırı ve
  her kayıt için `draw id=… written=… clipped=…` basılır. Gerçek: `MAP`
  `tempsave.btb` 87 -> `drawn=27 pixels=163079` (kimlikler 33..0); `VT` `73.btb`
  1638 -> `drawn=2`. Düzlem 640x480 sahne yüzeyinin görünen 640x400 kuşağıdır
  (`0x42b280` `0x4b000` kelimeyi siler, `0x41045b` arka planı `+0x6400` = 20
  satıra yazar, `0x40241c` `y-0x14` sınar), bu yüzden çizim `y-0x14` iledir.
  Arka plan/panorama katmanları `JPEG_DECODE.md` ile kapandı: `--background
  <jpg>` (tam 640x400) ve `--panorama <jpg> [--panorama-x N]` kayıt
  geçişinden önce yüzeyi doldurur. `--panorama` artık oynatıcının da kullandığı
  `PanoramaView` sınıfından geçer (`0x4236b5` dikiş satırları + `0x401ddf` +
  `0x4242bd` örnekleme); `0x4240ae` ters eşlemesi `panorama-view-test` ile
  örnekleme haritasına karşı doğrulanır (`analysis/rea/PANORAMA_VIEW.md`).
  Kapsam dışı: `+0x09 & 0x20` "snap" geçişi,
  `+0x09 & 0x80` hedef-okuma karışımı (`0x42b5b2`), mode-1 kare dizisi, `+0x3c`
  iz listesi ve geçiş/karışım katmanları. Kanıt: `analysis/rea/SCENE_COMPOSE.md`,
  `analysis/rea/JPEG_DECODE.md`.
- JPEG çözücü eklendi: `0x40f135` statik bağlı **IJG libjpeg 6a**'yı çağırır
  (`0x43de70` sürüm 61 / yapı 424), `0x40f080` mod 6 satırı `r>>3, g>>2, b>>3`
  ile RGB565 yapar. Port `src/jpeg.cpp`: katı işaret taraması, kanonik Huffman,
  DQT/DHT/DRI, baseline + progressive, jidctint IDCT, 2:1 için 3:1 üçgen
  örnekleme, `jdcolor` sabit nokta dönüşümü. Gemideki **283 JPEG**'in
  (215 `backs/` 640x400, 68 `nodes/` 2496x768; hepsi SOF0 4:2:0) tamamı çözülür.
  `jpeg-test` sentetik baseline/progressive akışları, dokuz reddi ve
  `analysis/rea/jpeg_sample/` doğrulama hash'lerini kilitler; `--scan` modu
  külliyat sayımı içindir.
- Animasyon kare akışı da çözüldü: kare verisi `0x40bbfd`'nin koşu (run)
  akışıdır — `0x0000|n` n adet literal piksel, `0x4000|x` satır başlangıcı,
  `0x8000|n` tek renkli n piksel, `0xC000|n` saydam boşluk; `n` alt 14 bit ve
  işaretli okunur. Her satır bir `0x4000` ile açılır, akış bir kapanış
  `0x4000`'i ile biter (`row == height`). `0x40be80` çözücü değil **yükleme
  anı renk geçişidir**: aynı akışta yalnızca 0/2 komutlarının piksel
  sözcüklerine `0x4164a0`'ı (`(p & 0x1f) | ((p & 0xffe0) << 1)`, yani
  **RGB555 -> RGB565**) uygular. Port: `decode_anims_frame` -> `DecodedFrame`
  (piksel + kapsama maskesi; komut 3 hiçbir şey yazmaz) ve
  `AnimsLibrary::decode`. Doğrulama: dosyalardaki **2.846/2.846 kare** temiz
  çözülüyor (64'ü meşru biçimde boş). `--trace` artık oluşturulan kayıtlar için
  `record_frame id=… index=… size=… covered=…` basıyor; `anims-test <ağaç>
  --render <ad> <kare>` kaba bir parlaklık haritası veriyor. Kanıt:
  `analysis/rea/ANIMATION_ASSET.md`.
- Animasyon varlıkları çözüldü: `0x40fe70` `anims\<ad[0]><ad[1]>\<ad>.rle`
  yolunu kurar, olmazsa soneki `.avi` yapıp `0x41cb13`'e düşer. `.RLE` başlığı
  10 bayttır (`u16` imza → kayıt `+0x28`, `u32`+`u16` kullanılmıyor, `u16` kare
  sayısı); her kare 26 baytlık başlık (`+0x02` genişlik, `+0x04` yükseklik,
  `+0x06/+0x08` x/y ofseti, `+0x16` sözcük sayısı) ve `words*2` bayt taşır.
  `0x410078` `0x40ffee(record, kare_sayısı)` çağırır (`+0x14`), ilk karenin
  ofsetlerini kaydın x/y'sine ekler ve `0x91`'in yürüdüğü kare düğümlerini
  doldurur. `--anims <dizin>` verilirse kayıt oluşturulurken varlık yüklenir;
  dosya yoksa `0x40fe70` başarısız olur ve komut gövdesi atlanır
  (`anims_load_failures`). `0x91` artık `0x40cf73` gibi gerçek kare indeksini
  yürütür (0xffff temizler; liste sonunu aşan yürüyüş `frame_overflow_logs`
  sayılır ve son karede kalır), `0x64` listeyi 0'a sarar. Doğrulama: dosyadaki
  922 `.RLE`'nin tamamı tam olarak ayrışıyor; 77 kayıttaki 448 düğüm yuvasının
  447'sinde `+0x14` varlığın kare sayısına eşit, ofseti sıfır olmayan
  **429/429** yuvada `x == ofset_x` ve `y == 0x14 + ofset_y`. Piksel akışı
  (`0x40be80`/`0x4164a0`) ve çizim kapsam dışı. Kanıt:
  `analysis/rea/ANIMATION_ASSET.md`.
- `DAT_004649e8` artık **tek** kayıt tablosudur: `SceneActorState`/`actors` ile
  `SceneNodeState`/`nodes` birleşti (`SceneRecordState`/`records`). Kaydın var
  olup olmadığı oyundaki `engine/<sahne>ca` tablosundan (`--ca`) belirlenir:
  `0x40fd7b` sahne başında tip 2/3 kayıtlarını, `0x431b0f` ise tip 0/1
  kayıtlarını `0x40fe70` ile yükler; diğer tiplerde (veride `0xff`) kayıt NULL
  kalır, `0x431b0f` 1 döndürür ve yönlendirici komut gövdesini **atlar**, betik
  sürer (`skipped_node_commands`). Kayıt tablosu dışı bir id `missing_node_asset`
  ile durur (özgünde `DAT_0047dd70[id]+8` NULL okuması). `0x64` de dahil tüm
  kayıt komutları bu kapıyı kullanır (`0x42db22` `case 100:`); `--ca`
  verilmezse eski davranış korunur (`unresolved_node_asset`). Kayıtların hangi
  alanlarının kayıttan geri yüklendiği `0x426b9d`'nin kopyalama listesidir.
  Kanıt: `analysis/rea/RECORD_TABLE.md`.
- Son yedi engelleyici opcode da kapatıldı; artık **ulaşılan her opcode için bir
  port dalı var** ve census engelleyici listesi boş. `0x79` betiği bitirir:
  yönlendirici sonucu 1'dir, `0x42d594` bunu "`0x484e28`'e adresi yaz ve dur"
  olarak uygular; `DAT_0045f490` yalnızca 0 iken 1 yapılır
  (`script_end_requested`). `0x7f` `DAT_00464f60[ham(0)]` kaydının
  `+0x00 − +0x04` farkını (32 bit çıkarma, 16 bite kırpma) ham operand 1'in
  yerel değişkenine yazar; kayıt NULL denetiminden **önce** okunduğu için eksik
  yuva `missing_media_record` ile durur. `0x86` kayıttan gelen düğümün `+0x04`
  alanını ham operand 1'den yazar ve operand 2..7'nin tip-2 olanlarına
  `0x460418`/`0x460428` maskesini uygular (`node_writes`; düğüm yoksa
  `unresolved_node_asset`). `0x8d` CD isteğidir (`0x431a69` → `0x40c7b4`,
  sürücü numaralandırma + birim denetimi); sonuç ana makine durumu olduğu için
  `disc_request` olarak durur. `0xcf` her tip-2 operand için `0x427a01` isteğini,
  `0xd0` `0x4278ab` durdur-hepsini kaydeder (DirectSound nesneleri kapsam dışı).
  `0xd3` `0x428cbd`'nin dokuz argümanlı biçimidir: mod 0, metin yok, üç
  (zaman aşımı ms, `0x431ec2` hedef kaydı) yuvası `SceneSoundSession` olarak
  tutulur. Ayrıca **düzeltme**: `0x4324b0` `+0x08` değil `+0x09` yazar
  (`0x43251d`, `mov BYTE PTR [ecx+0x9],al`), bu yüzden `0x64` artık `flags`
  alanını yazar ve `0x84`'ün `+0x09 & 0x20` iz kapısı gerçekten açılır.
  Kanıt: `analysis/rea/FINAL_OPCODES.md`.
- `<kayıt> --save-dump` artık ayrıştırmanın sonunda `load_save` sözleşmesini de
  çalıştırıp `restore objects=... dialogue_records=... dialogue_stacked=...`
  satırını basar; bozuk derinlik/kimlik dump'ta da görünür.
- `byzantine-cc <kayıt> --save-dump [--lcc <SHOW.LCC>] [--cc <sahne.cc>]`,
  `savegame/NN.BTB` biçimini blok blok okur (`include/byzantine/save.hpp`,
  `src/save.cpp`, `parse_save`). Blok sırası ve boyutları yükleyici `0x425ab4`
  ile yazıcı `0x424dc0`'ın aynı sırayı kullanmasından, ayrıca 77 gerçek kaydın
  bayt-bayt sonuna kadar ayrıştırılmasından doğrulandı: 4 bayt CD numarası,
  bölüm adı, kayıt adı, 168×168 RGB16 küçük resim (0xdc80 bayt), sahne indeksi,
  0x15e sahne kaydı, `DAT_0047e2f0` (3000 sözcük), diyalog kanal derinlikleri
  (10×u32) ve yığınları (10×350×u32), diyalog bayrakları, düğüm tablosu,
  0x18 baytlık nesne tablosu (`DAT_00464300`), 0x3c ses tablosu + zinciri,
  0x14 medya tablosu (`DAT_00464f60`), 0x68 diyalog zincirleri ve notebook
  metni. `--lcc` sahne indeksini bölüm listesine çevirir; `--cc` verilirse her
  nesne kaydının `+0x0c` etiketi o sahnenin `resolve_control_flow` etiket
  kümesinde aranır (4.716/4.716 eşleşti). Eksik/taşkın girdi
  `std::runtime_error` ile reddedilir. `load_save(SceneState&, const SaveGame&)`
  aynı görüntüden `DAT_00464300` nesnelerini (`+0x0c` etiketi ve `+0x14`
  argümanı dahil), `DAT_00464f60` medya kayıtlarını, `DAT_00486220` ses
  kayıtlarını ve `DAT_004649e8` düğümlerini geri yükler; `+0x0a` nesne ve
  `+0x12` medya baytları yazıcısı olmadığı için bilinçli olarak yorumlanmaz.
  Değişken havuzu (`DAT_0047e2f0`), diyalog yığınları ve sahne kayıtları bu
  adımda geri yüklenmez. Kanıt: `analysis/rea/SAVE_FORMAT.md` ve
  `analysis/rea/SAVE_RESTORE.md`.

Debian'da üç program derlendi. RLE çıktısının altı karesi bağımsız Python çözümünün pikselleriyle birebir eşleşti. CD1 içindeki 19 `.ALL` arşivinin tamamı yeni ayrıştırıcıdan geçti. `CC.ALL` içindeki 44 sahne betiği ayrıştırıldı; bunlardan 2.070 sabit tıklama alanı ve 524 sabit arka plan komutu okundu. `CB.ALL` içindeki 44 kayıt da ayrıştırıldı; 376 adın 366'sı altı ISO'daki `backs` JPG dosyalarıyla eşleşiyor. Kalan 10 kayıt: beş `examine`, iki `exclip` özel kaydı ve `ALGDoor2`, `BCCountr`, `EAPhne2` adları. Bu kontroller dosya biçimi kapsamındadır; oyunun açılması veya macOS/Windows çalışması henüz doğrulanmadı.

Akış çözümlemesi 44 betikte 7.693 benzersiz etiket, 74 yinelenen etiket tanımı ve 5.747 atlama/çağrı buldu. Tüm hedef etiketler çözüldü. `0x72` alanları kapalı başlatılıyor; `0x6d` açıyor, `0x6e` kapatıyor. Etkin alanların gerçekten hangileri olduğu koşullara ve yürütme yoluna bağlı.

## Değişken eşlemeleri ve yeni oyun izi

`SHOW.LCC`/`SHOW.LCV`/`SHOW.LVM` okuyucusu 44 sahnede 1.622 yerel/genel
bağlantıyı ve 1.049 genel değişken adını doğruluyor. EA değişken 5 = `lastNode`
(genel 8), değişken 26 = `flowChk` (genel 74). `--variables <engine-dir>` bu
eşlemeleri gösterir. `--trace-new-game <etiket> <CB> <engine-dir>` açıkça yeni
oyunun sıfır genel havuzundan başlar; başlangıç betiğini yürüttüğü anlamına
gelmez. Normal `--trace` bilinmeyen geçmiş durumu sıfırla doldurmaz.

```sh
port/byzantine-cc port/output/cc_all/EA --variables analysis/rea/engine_sample/copy2hd/engine
port/byzantine-cc port/output/cc_all/EA --trace-new-game 602 port/output/cb_all/EA analysis/rea/engine_sample/copy2hd/engine
```

Yeni oyun anlık durumuyla 475 → 595 → 602 testi, eksik değişken yerine
`0xb9` panorama yükleyicisine ulaşıyor. Tam panorama dönüşü ve sahneler arası
genel durum henüz uygulanmadı. Kanıt: `analysis/rea/VARIABLE_METADATA.md`.

## Derleme ve kullanım

Üç sistemde CMake ve C++17 derleyicisiyle:

```bash
cmake -S port -B port/build
cmake --build port/build
```

Bu makinede CMake kurulu olmadığı için bu yol henüz çalıştırılmadı. Debian'da `make` ile doğrulanan yol:

```bash
cd port
make
./byzantine-rle ../analysis/rea/HOTBAR.RLE output/hotbar
./byzantine-all ../analysis/rea/engine_sample/copy2hd/engine/Byz.all output/byz_all
./byzantine-cc output/cc_all/START
./byzantine-cc output/cc_all/AL --hotspots
./byzantine-cc output/cc_all/AL --backgrounds output/cb_all/AL
./byzantine-cc output/cc_all/EA --flow
./byzantine-cc output/cc_all/EA --labels
./byzantine-cc output/cc_all/EA --trace 475 output/cb_all/EA

# Sahne oynatıcı: veriyi ilk çalıştırmada çıkarır, sonra pencereyi açar.
#   ./scene.sh <oyun-dizini> [sahne] [etiket] [save]
./scene.sh ".." MAP 87 tempsave.btb          # harita: 27 kayıt + tıklanabilir alanlar
./scene.sh ".." VT 497 72.btb                # ses: 4.7 sn voTins1, betik bekler
./scene.sh ".." CLOCK --entry 70.btb          # metin: saat gövdesine tıkla (inceleme yazısı)
# Başsız doğrulama (pencere açmadan):
SDL_VIDEODRIVER=dummy ./scene.sh ".." MAP 87 tempsave.btb --click 34 --shot /tmp/map.ppm
SDL_VIDEODRIVER=dummy ./scene.sh ".." VT 497 72.btb --print-audio
# İmleç tanılaması: oyunun o nokta için seçtiği imleci kurar ve yazar.
SDL_VIDEODRIVER=dummy ./scene.sh ".." EA 602 26.btb --cursor-at 282,319 \
    --cursor-at 170,430 --shot /tmp/ea.ppm
```

### İki sahne arasında gidip gelme (MAP ↔ MR)

`MAP` başlangıç etiketi 87'de 11 tıklama alanı açar; bunlardan biri (ekranda
yaklaşık `190,300`, alan 47) bir oda etiketini çalıştırıp çıkış alanını (aynı
dikdörtgende 13) etkinleştirir; ikinci tıklama `0x73` ile `MR`'ye (`index=27`)
geçer. `MR` girişi `MRDOORCL.jpg` arka planını ve 2 alanı açar; sağ alttaki
alan (ekranda yaklaşık `589,330`, alan 38) `0x73` ile `MAP`'e (`index=25`) döner.

```bash
# Pencere: MAP'ta 190,300'e iki kez, MR'de 589,330'a bir kez tıkla
./scene.sh ".." MAP 87 tempsave.btb

# Aynı yol başsız (üç tıklama, son kare /tmp/roundtrip.ppm):
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
  ./scene.sh ".." MAP 87 tempsave.btb --no-audio \
  --click-at 190,300 --click-at 190,300 --click-at 589,330 \
  --shot /tmp/roundtrip.ppm
```

Beklenen satırlar (değerler değişmez):

```text
click at 190,320 slot 47 action 246
click at 190,320 slot 13 action 257 scene_switch=27
travel index=27 scene=mr from=MAP pool=3000 locals=300 pool_changed=1 (2) … record_preload=0
entry scene=MR steps=100 stopped_at=89 hotspots=2 records=0 …
click at 589,350 slot 38 action 96 scene_switch=25
travel index=25 scene=map from=MR pool=3000 locals=300 pool_changed=4 (10,79,80,718) … record_preload=17
entry scene=MAP steps=158 stopped_at=137 hotspots=11 records=17 …
```

`click_result` basılmadan `stopped: <neden>` yazıyorsa tıklama **başarısız**
sayılır; kısmi durum uygulanmaz ve geçiş yapılmaz (ör. `--lcx` verilmezse
`stopped: unresolved_scene_table=31`). MAP sonrası harita çiziminde `0x40fd7b`'nin
17 tip-3 kaydı önceden yüklenir, `0x43b216`'nın `0x20`/`0x40` çizim kapısı
uygulanır (`analysis/rea/SCENE_TRAVEL.md` §7).

Windows'ta Visual Studio Developer Command Prompt içinde:

```bat
cl /std:c++17 /EHsc /Iinclude src\main.cpp src\rle.cpp /Fe:byzantine-rle.exe
cl /std:c++17 /EHsc /Iinclude src\all_main.cpp src\all.cpp /Fe:byzantine-all.exe
cl /std:c++17 /EHsc /Iinclude src\cc_main.cpp src\cc.cpp src\cb.cpp /Fe:byzantine-cc.exe
```

Windows ve macOS derlemeleri burada henüz çalıştırılmadı. Kaynak ISO ve oyun dosyaları değiştirilmez; örnek girişler yalnızca analiz için çıkarılmış kopyalardır.

`byzantine-play` için SDL2 geliştirme paketi gerekir (`libsdl2-dev`); Makefile
`pkg-config` ile bulur. Arka planlar gömülü JPEG çözücüyle açılır, dış süreç
yoktur. Ses SDL2 audio cihazına gider; cihaz yoksa oynatıcı sesi atlayıp
pencerede çalışmaya devam eder (`--no-audio` ile de kapatılabilir). Başsız
ortamda `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy` ile aynı kod yolu
sınanabilir.

## Devam eden araştırma

`Byz.all` içinden Türkçe oyun metinleri çıkarılabiliyor. `SHOW.LVM` içinde değişken ve işlev adlarına benzeyen kayıtlar var. `CC.ALL` sahne betiklerinin sözdizimi ve sabit tıklama alanları artık ayrıştırılıyor. Sonraki hedef işlem kodlarının kalan anlamlarını ve yorumlayıcıyı REA ile çözmek; ardından sahne çizimi, girdi, video ve ses katmanlarını ekleyerek ilk oynanabilir bölümü oluşturmak.

Arka plan yolu da belirlendi: `0x69` sahne komutu geçiş/çizim işlevine gidiyor; düğüm adından `backs/<ilk iki harf>/<ad>.jpg` biçiminde dosya açılıp 640×400 görüntü tamponuna çözülüyor. CD1'de bu klasörde 215 JPG bulundu ve bir örneğin boyutu doğrulandı. `CB.ALL` ile sabit düğüm kimliklerinin adları eşlendi. Sahne çizicisi, özel envanter yolları ve çalışma anındaki koşullar henüz uygulanmadı. Ayrıntı ve REA kanıtı `analysis/rea/README.md` içinde.

[`EA-first-background.svg`](../analysis/rea/scene_sample/EA-first-background.svg), `EA` betiğinde etiket 475'in yüklediği `EAEmShMs.jpg` arka planını ve aynı blokta açılan 76/93 numaralı alanları gösteren tanısal bir örnektir. JPG tamponu, 640 piksel genişliğinde 16 bit görüntünün 20 satırlık ofsetine yazıldığı için görsel SVG'de `y=20` konumundadır. Bu SVG oyunun çalışma anındaki tüm etkin alanlarını veya geçişlerini temsil etmez.

`--trace 475` üç komut yürütüp arka plan 0 ile 76/93 alanlarını veriyor; `AL` betiğinde `--trace 89` de arka plan 0 ile 4/5 alanlarını veriyor. İki yol da dönüş komutunda duruyor. `--trace` bilinmeyen işlem kodunu sessizce atlamaz; böylece çözülen parçanın sınırı açıkça görülür.
