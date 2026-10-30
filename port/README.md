# OpenByzantine — taşınabilir C++17 oyun motoru

Güncel kısa devam kaydı: [`../CONTINUE.md`](../CONTINUE.md).

## Üst menü (top bar) ve ekranları

Fare ekranın üst 20 pikseline girdiğinde oyunun kendi `setupart/TOP_BAR.TGA`
çubuğu görünür; dört düğme (`HOTBAR.RLE`/`DWNBAR.RLE` kareleri) vurgulanır ve
tıklanınca `0x41e289`'un ekranı açılır:

- **YÜKLE/KAYDET** (mode 1): `autosave.btb` + `btb.sav` slot listesi; OYUN
  YÜKLE seçilen `.btb`'yi gerçek kayıt durumuyla yükler (giriş betiği yeniden
  çalıştırılmaz; arka plan/panorama kayıttan gelir), YENİ OYUN `new.can`'ı
  açar, OYUN İPTAL onaylı siler, OYUN KAYIT tam `0x424dc0` görüntüsü yazar.
  Kayıt adı için gerçek MESSAGE 0x3c istemi ve CP1254 metin girişi kullanılır;
  başka diskten yüklemede o diskin animasyon katmanı hazırlanır.
- **NOTLAR** (mode 3): `s_note00.jpg` sayfası; sağ sayfaya tıklamak TEMAS
  LİSTESİ (notebook.000 kişileri) ve defter sayfalarına geçer. Defter
  sayfalarında yer adı (0x421690 tablosu), konuşmacı ve gerçek satır metni
  (`0x41db33` → kayıttaki block-6 `Dialog` slotunun `.byz` dosyası) gösterilir.
- **KEŞİF** (mode 2): `status.000`'a göre `MESSAGES.TXT` 0x34..0x3a satırları;
  jenerik (`CREDBK.JPG` + `text/CREDITS.TXT` kaydırması), dört simülasyon
  sahnesi (`vtdisc` → `vt` zinciri dahil) ve intro/epilogue filmleri (mevcut
  FFmpeg klip yolu). Sağdaki İPUCU sekmesi HINTS.JPG + gerçek Hints1
  konuşmasını çalıştırır (`0x48` IFREPLY eklendi; klipler CD'de yoksa metinle
  devam eder, tablolar yoksa defter kaydına düşer).
- **TERCİHLER** (mode 0): yedi tercih; değişiklik `savegame/gamepref_000`
  dosyasına yazılır (`0x41f7be`).

ESC ekranı kapatır, OYUNA DEVAM oyuna döner, OYUNDAN ÇIK onay ister. Masaüstü
tanılamaları: `--menu <0..5>`, `--menu-click-at <x>,<y>`, `--menu-hover
<x>,<y>`, `--menu-text <utf8>`, `--menu-key enter|escape|backspace`,
`--menu-shot <ppm>`, `--credits-frames <n>`, `--credits-shot <ppm>` ve
`--bar-show <0..4>`. Ayrıntı ve sınırlar:
[`../analysis/rea/TOP_MENU.md`](../analysis/rea/TOP_MENU.md).

## Disklere göre animasyon seçimi

Launcher kayıt dosyasındaki disk numarasını okur ve o diskin animasyonlarını
ayrı bir `disc-media/` önbelleğinde tutar. Aynı adlı dosyaların farklı disklerde
farklı içerikleri olabilir; seçilen diskin dosyaları ortak çıkarılmış klasöre
önceliklidir. İlk açılışta bir kez ek çıkarma yapılır. Doğrudan CLI kullanırken
`--anims-overlay <disk-klasoru>` aynı önceliği sağlar. Oturum içindeki disk
isteme komutları henüz otomatik disk değiştirme uygulamaz.

## Tam kurulum (CD istemeyen oynatma)

Normal `scene.sh` akışı eksik bir diyalog filmini gerektiğinde ISO'dan çıkarır.
Bunu bir kere yapıp diskleri oynatma sırasında hiç istememek için:

```sh
./port/full_install.sh "<oyun-klasörü>" /tmp/byz-scene
```

Altı diskin `dialog/`, `video/` ve `avi/` ağaçlarını hazırlanmış veri
klasörüne kopyalar; `scene.sh`'in `--media-isos` manifesti yerinde kalabilir,
dosyalar zaten bulunduğu için ek çıkarma yapılmaz.

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


## Gelişmiş ayarlar (TERCİHLER / F10) ve dil paketleri

Portun kendi ayar ekranına iki yol vardır: üst çubuk → `TERCICHLER` →
`GELİŞMİŞ AYARLAR` alt çekmecesi (native giriş; altı grup kısayolu, varsayılan
açık, `ESC`/ayar tuşu TERCICHLER'e döner) ve sahneden `F10` kısayolu. Orijinal tercih ekranına ve `gamepref_000` dosyasına
dokunulmaz. Ekran sekmelere ayrılmıştır: `GÖRÜNTÜ`,
`SES`, `DİL`, `OTURUM`, `TUŞLAR` ve salt-okunur `ORİJİNAL`. Native sayfada
arka plan `PREFFS.JPG`, `F10` sayfasında karartılmış `CREDBK.JPG`; çerçeve ve
vurgular oyunun altın/deri paletini kullanır. Native sayfada açıklamalar sol
defter sayfasına, `F10` sayfasında sağ paneledir. `PORT` bu uygulamanın ayarı,
`OYUN` ise orijinal `gamepref`/TERCICHLER değeridir. `ORİJİNAL` sekmesinde orijinal
oto kayıt, video boyutu, ses/müzik, konuşma notu, yönlendirici ve yer tanıtım
tercihlerinin yalnızca mevcut durumu gösterilir; bunlar üst çubuktaki
TERCICHLER ekranında değiştirilir. `Tab`/`Shift+Tab` grup, yukarı/aşağı satır,
sol/sağ değer değiştirir; `ESC`/`F10` kapatır:

- pencere: 4:3 pencereli / çerçevesiz tam ekran / büyütülmüş
- ölçekleme: normal pencerede otomatik 4:3 fit; diğer modlarda fit veya tam sayı (pixel-perfect)
- VSync
- ana ses / konuşma / ambiyans / efekt ses seviyeleri
- altyazı aç/kapat (yalnızca diyalog bandı; inceleme/defter metni gizlenmez)
- hotspot tanılaması (kapalı / imleç / tümü)
- metin dili ve seslendirme dili (bulunan dil paketleri)
- imleç ölçeği (1x..4x), odak kaybında sessize alma
- otomatik kayıt, görülen satırı atlama, gamepad ve port tuşları
  (tuş satırında `Enter`, sonra yeni tuş; `ESC` iptal)

`F11` ve `Alt+Enter` pencere/çerçevesiz tam ekran arasında
geçer. `F5` hızlı kayıt (`savegame/quicksave.btb`), `F9` hızlı yükleme, `F8`
otomatik kaydı yükleme (`autosave-port.btb`), `F3` diyalog geçmişi, `F4`
görülen satırı atlama, `F12` ekran görüntüsü (`port/output/screenshots/`,
`BYZ_SCREENSHOT_DIR` ile değişir).

Gamepad: sol çubuk imleci taşır, A tıklar, B `Esc`, Start ayarlar, X geçmiş,
Y görülen-satır atlama.

Diyalog geçmişi son 400 satırı tutar; `seen_dialogue.txt` (INI'nin yanında)
görülen satır anahtarlarını saklar. `F4` açıkken daha önce görülmüş satırlar
metin ve AVI olarak atlanır; ilk kez görülen satır atlanmaz. Otomatik kayıt her
sahne geçişinden sonra `autosave-port.btb` yazar. Bunlar port özellikleridir;
orijinal kayıt/tercih dosyalarına karışmaz. Oyun kapalıyken otomatik kayda
dönmek için `./port/continue.sh "<oyun-klasörü>"` kullanılabilir; kaydın sahne
adını `SHOW.LCC` üzerinden çözüp normal başlatıcıyı o save ile açar.
`continue.sh` artık `--restore-save` ile kaydın görüntüsünü ve durumunu doğrudan
yükler; giriş etiketini yeniden çalıştırmaz. Portun yazdığı kayıtların yanındaki
`.btb.byzport` dosyası kamera eğimi, rastgelelik durumu, konuşma hafızası ve
incelenen eşyadan dönülecek sahneyi korur. Kayıt taşırken iki dosyayı birlikte
taşıyın. Ek dosyası olmayan özgün kayıtlar da yüklenir.


Ayarlar `--config <ini>`, yoksa `$BYZ_CONFIG`, yoksa çalışma dizinindeki
`byzantine.ini` dosyasına yazılır; `scene.sh` varsayılan olarak
`port/byzantine.ini` geçirir. `--no-config` yükleme/kaydı kapatır. Oyun hâlâ
640x480 mantıksal çözünürlükte çizilir; 16:9 ekranda 4:3 görüntü siyah
bantlarla gösterilir, gerilmez.

Dil paketleri `--text-lang <kod>`, `--voice-lang <kod>` ve `--languages <dir>`
ile seçilir. `port/language_pack.py`, hazırlanmış bir yerelleştirilmiş veri
ağacını taban ağaçla hash bazında karşılaştırıp yalnızca farklı dosyaları
`<base>/lang/<kod>/` altına kopyalar; tek tek resim tespiti/OCR yapmaz. Metin
ve ses ayrı seçilebilir; filmlerde ayrı altyazı izi olmadığı için film dili ses
paketini izler. `port/language_audit.py`, hazırlanmış taban ve dil ağaçlarını
karşılaştırıp dil işareti, farklı dosya sayıları, BYZ/CCF/CE satır uyumu ve
boyut raporu üretir; hiçbir şey yazmaz:

```sh
./port/language_audit.py --base /tmp/byz-scene \
    --pack en=/tmp/byz-scene-en --pack de=/tmp/byz-scene-de
```

Kurulu paketleri ve dil işaretlerini görmek için
`./port/byzantine-play --list-languages --backgrounds /tmp/byz-scene` (veya
`--languages <dir>`) kullanılabilir. Ayrıntı:
[`../analysis/rea/PORT_SETTINGS.md`](../analysis/rea/PORT_SETTINGS.md).

### Sunulan kare izi (`present`)

Ekranda bir an görünen yanlış/başka bir resmi bulmak için her
`SDL_RenderPresent` ayrıntı loguna (`run-....trace.log`) yazılır:
`present index= tag= scene= label= layer= dialogue= bar= hash= prev_hash=
changed= prev_held_ms=`. `layer` o anda çizilen katmanı (arka plan JPEG'i veya
panorama düğümü + yaw/pitch), `detail` bir video karesiyse tam kareyi
(`SMEst1:65/65:f0065.jpg`), `prev_held_ms` ise **önceki resmin ekranda kaldığı
süreyi** verir. Sahne yüklemesi ve video bitişi 800 ms'lik bir yakalama penceresi
açar; `travel_begin`/`travel_loaded` ve `present_capture` satırları yüklemeyi
sınırlar. Animasyon her karede log basmaz; resim/katman değişiminde ve 5 s'lik
kalp atışında yazar. Koşu sonunda `present_summary frames= logged= dumps=`
basılır. Ayrıntı: [`PRESENT_TRACE.md`](../analysis/rea/PRESENT_TRACE.md).

Her `present` satırında artık `overlay=0|1` de var: hotspot çerçevesi o karede
çizildi mi. Hangi sunucunun ekranı sahiplendiği `screen_content now=…
previous=…` satırlarıyla görünür (`scene`, `conversation`, `clip`, `menu`).
Politika tek yerde: `Display::present_image` (`ScreenContent`). Sarı çerçeve
tanılaması **varsayılan kapalı**. Haritada imleç bir hedefin üzerine gelince
hedefin kendi seçim etiketi çalışır: `*Hi` vurgu resmi çizilir ve o hedefin
seyahat nesnesi açılır (`hover_select`), böylece **tek tıkla** oraya gidilir.
Vurgu geçicidir: `hover_select` etiketten önce tüm `SceneState`/`SceneTrace`
kopyasını alır, imleç hedefi terk edince (veya pencere odağı gidince)
`restore_hover_preview` bu kopyayı aynen geri koyar; böylece vurgular birikmez.
Log: `hover_highlight slot=N label=… at=x,y` ve
`hover_highlight slot=-1 restored`. Tam-ekran "iptal" nesneleri hover'da
çalışmaz; seyahat/diyalog/klip/ses/blocker üreten etiketler reddedilir
(`skipped reason=not_a_highlight`).
`--hotspots-hover` imlecin altındaki alanı, `--hotspots` hepsini çizer; H
aç/kapa. Klip ve menü ekranlarında asla çizilmez;
klip (ses kuyruğu dahil) ve menü ekranları asla çerçevelenmez. Tıklama bitişi de
tek yerde: `commit_click` (pencere, `--click`, `--click-at`, `--run-label`).
Ayrıntı ve gelecek çalışma kuralları: `AGENTS.md`,
[`PRESENTATION_ARCHITECTURE.md`](../analysis/rea/PRESENTATION_ARCHITECTURE.md).

Karelerin kendisini de saklamak için (video kareleri hariç, tekrarsız, varsayılan
en fazla 40 PPM):

```sh
BYZ_PRESENT_DUMP=/tmp/byz-present ./port/scene.sh ".." MR 0 30.btb --entry
# veya doğrudan oyuncuya: --present-dump <klasor> [--present-dump-max N]
```

Her yazılan kare terminale `present_dump file=... layer=...` olarak basılır.

Bir tıklama `0x73` ile bitiyorsa (sahne değişecekse) tıklamanın kendi karesi
artık sunulmaz: hedef sahneyi `travel_to` sunar. Böylece bir site videosundan
sonra eski harita karesi araya girmiyor; video son karesi hedef sahne hazır
olana kadar ekranda kalıyor (`PRESENT_TRACE.md`, Topkapı girişi).

### Sahne geçişleri (0x40186e / 0x431ec2'nin fade/blend ailesi)

Hedef sahne artık tek karede gelmiyor: `0x40186e` (panorama düğümü) ve
`0x431ec2` (`0x69` arka planı) aynı blend ailesini çağırıyor ve port bunu
oynatıyor. Kod script'ten gelir (`0x69` operand 1 ve `0xb9` operand 3 →
`0x432133`, ya da önceki `0xbd`'nin bıraktığı ve `0x73`'ü aşan `DAT_00460664`).
Gönderilen kayıtlar çoğunlukla **100** ("tek karede çiz") diyor; port orada kendi
kodunu kullanıp `source=default` diye loglar, script'ten gelen kod ise
`source=background`/`source=panorama` olarak görünür.

```sh
# varsayılan: geçişler açık (veri 100 dese bile port kodunu kullanır)
#   --transition 0x64       orijinalin "hemen çiz" davranışı (veriye sadık)
#   --transition 0x66       kodsuz yüklemelerde 27 fazlı örgü
#   --no-transitions        geçişleri tamamen kapat
#   --transition-shot <dir> her ara kareyi PPM olarak yaz (kanıt)
```

Log: `transition code=0x65 kind=fade8 source=default steps=8 step_ms=16
elapsed_ms=139` ve her `0x69` için `background_load instruction=… id=…
transition=0x…`; ara kareler detay izinde `tag=transition
detail=code=0x65 step=3/8` olarak görünür ve hotspot çerçevesi onlara çizilmez.
Ayrıntı: [`TRANSITIONS.md`](../analysis/rea/TRANSITIONS.md).

### Klip sesi son-kırpı koruması

Bazı AVI'ların son ses tamponu ses değil: `NBEST1.avi` (Nargile Kahvesi site
videosu, altı CD'de de aynı dosya) 12.000 sn'de başlayan 67 ms'lik tam ölçekli
gürültüyle bitiyor; görüntü aynı anda siyaha dönüyor. Oynatınca "pat" sesi
çıkıyor. `byzantine::trim_torn_audio_tail` yalnız bu tür *yırtık* kuyruğu keser
(tek örnekli işaret sıçraması ≥0,45 FS, öncesi sessiz, arkası tam ölçeğe yakın ve
sona kadar süren patlama) ve kesimi loglar:

```
video_audio clip=NBEst1 tail_guard trimmed_at_ms=11999 removed_ms=67             flip_at_ms=12011 peak=32765 reason=single-sample-sign-flip
```

Klip görüntüsü yine 181 kare oynar; sadece bozuk kuyruk sessizleşir.
`--no-audio-tail-guard` korumayı kapatıp dosyanın ham sesini dinletir (A/B).
Kanıt: [`AUDIO_TAIL_GUARD.md`](../analysis/rea/AUDIO_TAIL_GUARD.md).

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
  (`--anim-pass-hz`, varsayılan 60; gerçek adım hızı `pass / (delay+1)`).
  Gecikme tablosu (varsayılan `+0x04` = 10, değerler 1..60) 60 Hz'lik bir
  tasarım ölçeğine oturur; 250 Hz tek bir TK kaydı için seçilmişti ve
  delay-1 karakterleri (NB/SM/HL) yaklaşık 4x hızlandırıyordu. Detay:
  [`ANIMATION_RATE_GLOBAL.md`](../analysis/rea/ANIMATION_RATE_GLOBAL.md).
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

SDL2 ve C++17 derleyicisiyle CMake üzerinden (Linux üzerinde doğrulandı):

```bash
cmake -S port -B port/build
cmake --build port/build
```

CMake derlemesi ve 14 CTest testi 29 Ekim 2026 tarihinde geçti. Aşağıdaki `scene.sh` başlatıcısı `port/` içindeki araçları kullanır; başlatıcı için Make ile derleyin:

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

Gameplay regression automation: [`tests/gameplay/README.md`](tests/gameplay/README.md)
describes `make test-gameplay DATA=/tmp/byz-scene`, private save/config handling,
soak runs, snapshot assertions and strict original-reference comparison.

### ISO dizinini bir kez tanımlama ve otomatik disk erişimi

Altı ISO aynı dizinde veya onun altındaki CD klasörlerinde bulunabilir.
Dosya adlarını değiştirmeniz gerekmez; diskler içlerindeki `CD.BYZ` ile tanınır.
İlk çalıştırmada dizini belirtin:

```sh
./port/scene.sh "/oyun/ISO-dizini"
```

Başarılı kurulum dizini `port/media_sources.json` dosyasında hatırlar.
Sonraki açılışlarda `./port/scene.sh` yeterlidir. Eski sahne/kayıt komutları
çalışmaya devam eder; hatırlanan dizinle belirli sahne açmak için ilk argümanı
boş bırakabilirsiniz: `./port/scene.sh "" MAP 87 27.btb`.
`BYZ_MEDIA_CONFIG` farklı bir medya yapılandırma dosyası seçer.

Oyun disk istediğinde ilgili ISO otomatik seçilir ve script devam eder.
Bağlama/sanal sürücü işlemi yapmanız gerekmez. Eksik veya birden fazla kopyası
bulunan disk açıkça bildirilir; çalışan ayar eksik bir taramayla bozulmaz.
İlk ihtiyaçta medya önbelleğe alınır. Diskler arasında aynı adlı dosyalar ve
çözülmüş video kareleri karışmaz; kayıt/yükleme seçili diski korur.

Gerçek ISO ve verilerle disk regresyonları:

```sh
make -C port test-disc-access DATA=/tmp/byz-scene \
  MANIFEST=/tmp/byz-scene/media_isos.txt OUTPUT=/tmp/byz-disc-test
```

Çıktı dizini yeni olmalıdır. Test, gerçek disklerin adları değiştirilmiş
bağlantılarını, script devamını, eksik disk hatasını, özgün MAP disk isteklerini
ve kayıt/yüklemeyi kontrol eder. Aynı adlı videolar için ayrı üretilmiş medya
örnekleri kullanır; bunlar özgün oyun eşdeğerliği kanıtı değildir.
Özgün EXE'nin Wine başlangıç testi de altı ISO'yu ayrı CD-ROM olarak hazırlayıp
CD sormadan menüye ulaşır; tam oyun akışı karşılaştırması henüz doğrulanmış
değildir. Ayrıntı: [disk erişimi raporu](../analysis/rea/AUTOMATIC_DISC_ACCESS.md).

### Otomatik oyun keşfi

`make -C port explore-game DATA=/tmp/byz-scene` komutu, hazır rota listesiyle
sınırlı kalmadan tıklanabilir alanları, envanteri, kamera yönlerini ve konuşma
cevaplarını ayrı dallarda dener. Süre/deneme/durum sınırlarında durur;
`port/output/explore-…/index.html` dosyası bulguları ve ekran görüntülerini
gösterir. Her bulgu başlangıç kaydı ve giriş dizisiyle yeniden çalıştırılabilir.
Bu, oyunun tamamının doğru çalıştığını kanıtlamayan sınırlı bir keşiftir.
Kullanım, sınırlar, medya profili ve tekrar çalıştırma:
[keşif rehberi](tests/exploration/README.md).

Uzun rastgele tarama için tek komut: `./port/explore_random.sh 2000`.
Bu 2000 ayrı sahne değil, en fazla 2000 oyuncu çalıştırmasıdır; eşya/cevap/eylem
yolları değişir ve benzersiz durum sayısı ayrıca raporlanır. Son tarama konumu
`port/output/EXPLORATION_LATEST` dosyasına kaydedilir.

2000’lik rastgele tarama artık mevcut numaralı kayıtların tamamını başlangıç
havuzu olarak kullanır (bu verilerde 49 kayıt, 18 sahne). İlk tur sahnelere
dağıtılır; sonraki yollar başlangıç kayıtları arasında dengeli seçilir.
Kayıtlı durum açılırken sahne giriş scripti yeniden çalıştırılmaz.

## Uzun yol, davranış ve medya test kampanyası

```sh
./port/explore_campaign.sh 2000
```

Bu komut2000 üst sınırına sahip karma mantık taramasına ek olarak, beklenen
sonucu denetleyen9 senaryoyu,32 rastgele ayar kombinasyonunu ve daha küçük
ses/video açık kontrolleri çalıştırır. Ayar denemeleri oyun eylemleri arasında
gerçek menü girişleriyle ayar değiştirir; pencere/ölçek, ses, odak, kısayollar,
kayıt/yükleme ve yeniden açılışta kalıcılığı kontrol eder. Ayrıca iki medya açık
ayar denemesi vardır. `--settings-cases 200 --settings-seconds 1800` ile ayar
taraması büyütülebilir. Bu ek denemeler2000 mantık çalıştırmasına dahil değildir.
Tek HTML raporu profil sonuçlarını ve gerçek toplam çalıştırmayı ayrı gösterir.
Karma taramada uzun yollar için ayrılmış seçimler vardır; varsayılan derinlik
24 adımdır. Kamera/eşya döngüleri daha az öncelik alır, yollar tamamen elenmez.
Sadece hızlı mantık taraması için `./port/explore_random.sh 2000` kullanılabilir.
Ayrıntılar: [`tests/exploration/README.md`](tests/exploration/README.md).


Hikâye/bulmaca zincirleri için kampanyaya `story` aşaması da eklendi. Dokuz
senaryo belirli özgün kayıtlardan eşya/bayrak/bulmaca sonuçlarına gider; sekizinde
ortada kaydedip ayrı bir oyun işleminde yeniden açarak devam eder. Tek başına:

```sh
make -C port test-stories DATA=/tmp/byz-scene OUTPUT=/tmp/byz-stories-new
```

Bu, yeni oyundan finale kadar tam çözüm testi değildir. Saatte bulunan devam hatası ortak motor kuralında düzeltildi; dokuz senaryo ve
sekiz kayıt/devam kontrolü artık geçiyor. Saatin üç aşaması, anahtar/cam ödülleri
ve odadan çıkış da doğrulanıyor. Ayrıntılar: `tests/exploration/README.md`.
