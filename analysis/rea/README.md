# REA ile ilk yürütülebilir dosya incelemesi

25 Ekim 2026. Hedef, oyunu modern Windows, macOS ve Debian'da çalıştıracak bir motor/uyarlama geliştirmek. Bu dizindeki analiz, özgün diski çalıştırmadan ve değiştirmeden yapılmıştır.

Güncel devam dizini: [`CONTINUE.md`](../../CONTINUE.md). Son odak çalışma:
[`SAVE_FORMAT.md`](SAVE_FORMAT.md), `savegame/NN.BTB` kayıt biçimini blok blok
belgeler: kayıt yükleyici `0x425ab4` ile yazıcı `0x424dc0` aynı sırayı kullanır
ve CD1'deki 77 kayıt bu sırayla sonuna kadar ayrıştırılır. C++17 okuyucusu
`port/src/save.cpp`, `byzantine-cc --save-dump` ve `save-test` eklendi; her
nesne kaydının `+0x0c` etiketi kendi sahnesinin CC etiket kümesinde doğrulanır.
Geri yükleme, `0xcc` ve `0x70` hâlâ açık işlerdir. Önceki odak:
[`PANORAMA_PROJECTION.md`](PANORAMA_PROJECTION.md), panorama açı dönüşümleri,
yaw ofseti, pitch sınırı ve kenar tamponu hesabını kayıtlı Ghidra çıktıları ve
dar disassembly aralıklarından kurtarır. C++17 yardımcıları ve testleri eklendi;
piksel eşitliği ve `0xb9` yükleyicisi hâlâ açık işlerdir. Kaynak kimliklerini
`python3 analysis/rea/verify_panorama_projection.py` ile doğrulayın.

## Araç ve kurulum

- Proje kökündeki `rea.sh`, gerekli Node/Java/Ghidra ortamını ayarlar. `./rea.sh` varsayılan olarak `BETRAYAL.EXE` analizini çalıştırır; örneğin `./rea.sh --help` veya `./rea.sh search "$PWD/analysis/rea/BETRAYAL.EXE" RLE --provider ghidra` başka REA komutlarını çalıştırır.
- REA 6.8.0 kaynağı: `tools/rea` (git commit `160d821`). Node.js 24.22.1 ile `npm ci`, `npm run build:cached` ve `npm link` başarılı oldu.
- Java: `/home/baturkacamak/.local/opt/rea-deps/jdk-21.0.13+9`.
- Ghidra: `/home/baturkacamak/.local/opt/rea-deps/ghidra_12.1.4_PUBLIC`.
- Codex MCP kaydı ve REA becerisi kullanıcı yapılandırmasına eklendi. MCP'nin bu oturumda görünmesi için Codex'in yeniden başlatılması gerekir. REA CLI bu oturumda çalışıyor.
- Analiz komutları için ortam:

  ```bash
  source "$HOME/.nvm/nvm.sh"
  nvm use 24.22.1
  export GHIDRA_INSTALL_DIR="$HOME/.local/opt/rea-deps/ghidra_12.1.4_PUBLIC"
  export JAVA_HOME="$HOME/.local/opt/rea-deps/jdk-21.0.13+9"
  ```

## Doğrulanan bulgular

- `BETRAYAL.EXE`, CD1'deki `copy2hd/BETRAYAL.EXE` dosyasından çıkarıldı. SHA-256: `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
- 32 bit x86 Windows PE. İçe aktarımlar arasında `AVIFIL32.dll`, `MSVFW32.dll`, `DDRAW.dll`, `DSOUND.dll` ve `WINMM.dll` var. Tam liste: `pe-imports.json`.
- Ghidra yaklaşık 1.397 işlev ve 1.047 dize tanımladı: `overview.json`.
- Kodda `nodes\\%s.jpg`, `setupart\\hotbar.rle`, `avi\\%s.avi`, `savegame\\%02d.btb` gibi yollar var: `format-strings.json`.
- `setupart\\hotbar.rle` başvurusundan `FUN_004200bb` yükleyicisine ulaşıldı. Bu işlev dosya başlığından kayıt sayısını alıyor; her kayıt için 26 bayt başlık okuyor, başlığın +22 ofsetindeki 32 bit sayı kadar 16 bit sözcük okuyor, ardından `FUN_0040be80` işlevini çağırıyor: `hotbar-rle-xrefs.json`, `hotbar-rle-caller.json`, `rle-loader-candidate.json`.
- `FUN_0040cb26`, ilk 10 baytı okuyup 8–9 ofsetindeki 16 bit değeri döndürüyor. Örnek `HOTBAR.RLE` dosyasında bu değer 5. İlk kaydın +22 ofsetindeki uzunluk 2688 sözcük: `rle-header-reader.json`, `HOTBAR.RLE`.
- `FUN_0040be80`, sözcüklerin üst iki bitine göre bazı 16 bit değerleri yerinde dönüştürüyor. Bu işlevin tek başına pikselleri açtığı kanıtlanmadı: `rle-decode-candidate.json`.

## İlk çalışan RLE çözücü

`rle_preview.py`, REA ile incelenen çizim yoluna göre 15 bit renkli RLE karelerini saydam PNG'ye çıkarıyor. `FUN_004090c4` yüklenen kareyi çizdirmek için `FUN_0040bbb0` ve `FUN_0040bbfd` çağırıyor. İkinci işlevde komut sözcüğünün üst iki biti ham piksel, yeni satır, tekrar edilen piksel ve atlama anlamına geliyor. `FUN_004164a0` renkleri ekranın 16 bit biçimine dönüştürüyor; özgün dosya rengi RGB555. REA kanıtları: `ev_fbf15ed23bf0aaa2ace4f9f415e86d52df3d201e700a095a3c327d485459bb95`, `ev_7490891080daf7bc0971c1af8f857ea4b569f3493975c22e744ed86f2d20841f`, `ev_18142dc7062fa06012ac391443d327c5c5af4332a3061d41f1c99f96985da364`, `ev_64b35db2b10b8b0cf1f5d2e731b04a9d32bfee2477c4af6a2a15a8bbb76dd0cb`.

```bash
python3 analysis/rea/rle_preview.py analysis/rea/HOTBAR.RLE analysis/rea/hotbar_frames
```

`HOTBAR.RLE` dosyasının 5 karesi ve `EAKEY.RLE` dosyasının 640×400 tek karesi başarıyla PNG'ye çevrildi. İlkinde Türkçe **YÜKLE/KAYDET** düğmesi, ikincisinde saydam arka plan üzerindeki anahtar görünüyor. Bu iki örnek çözücüyü doğruluyor; bütün RLE varyantlarının kapsandığı henüz gösterilmedi.

## Oyun verisi ve sahne komutları

`FUN_00411b4b`, `engine/*.ALL` paketlerinin dizinini okuyor. Dizin; 16 bit kayıt sayısı, sıfır sonlandırmalı adlar ve veri bölümüne göre 32 bit başlangıç/bitiş ofsetlerinden oluşuyor (`ev_3ebfbe148234712a019aa691f8cdf3501d5f1b2d905f1ac7098de35acf7077a9`). `Byz.all` içinden Türkçe diyalog ve ipuçları çıkarıldı. [`port/`](../../port/) içindeki C++ çözücü CD1'deki 19 `.ALL` paketini ayrıştırdı.

`CC.ALL` içindeki kayıtlar derlenmiş sahne komutları. REA'da `FUN_00412762` işlevi, baştaki 16 bit komut sayısını, ardından her komutun 8 bit işlem kodunu, argüman türlerini ve argüman verilerini okuyor (`ev_961516329fda54629fb59b3d818cb49b28be3122a552ef037259a378fdc01a23`). İşlem kodu argüman sayısı tablosu yürütülebilir dosyadan okundu (`ev_0c452b2e3ab8886e9627eec37b95ff26707255cc17cf2aa7718d5ad766e61e32`). C++ ayrıştırıcı `CC.ALL` içindeki 44 betiği dosya sonuna kadar doğruladı. İşlem kodlarının oyun içi etkileri henüz bilinmiyor; bu ayrıştırıcı bir yorumlayıcı değil.

REA'nın `FUN_0042db22` işlevindeki `0x72` dalı sabit tıklama alanlarını kuruyor (`ev_6fae0653eb9d17a560b88cf841ffbe1c2fc3b14d9c5f18ac7a7ef5e9853ca522`). C++ ayrıştırıcı bu tanımları listeleyebiliyor. `START` betiğinde 1, `AL` betiğinde çok sayıda alan bulunuyor; konumlar bazı panoramalarda 640 pikseli aşabiliyor. Bunlar statik betik tanımlarıdır, çalışma anında etkin oldukları doğrulanmadı.

REA'nın toplu çözümleme çıktısı [`pseudocode/decompile-index.json`](pseudocode/decompile-index.json) dizininde: 1.397 işlevin 1.237'sinin sözde kodu çıkarıldı; kalan 160 dış işlev. Bu sözde kod özgün C/C++ kaynağı değildir ve henüz derlenebilir bir motor oluşturmaz.

## Sahne arka planı çizim yolu

Sahne komutu `0x69`, `FUN_0042db22` içinde `FUN_00431ec2` çağrısına gidiyor. Bu işlev geçiş türünü seçip `FUN_0041045b` ile arka planı yüklüyor (`ev_faf2296e2234660299dfb1db2a9c86a8ff57075afca2cca24a6211fb42a473ae`). `FUN_0041045b`, sahne düğümü kaydının adından `backs` altındaki JPG yolunu oluşturuyor; envanter inceleme durumu için ayrı bir yol kullanıyor (`ev_a548faa896464e362cc257b0edde874482d2d9bb6a4a880c4281b0fd5c0eb54b`). `FUN_0040f135` JPEG'i çözerek 640×400, 16 bit renk tamponuna aktarıyor. CD1 ISO'sunda `backs` altında 215 JPG var; `backs/bc/BCAKALIN.JPG` örneği gerçekten 640×400.

`CB.ALL`, 44 sahne için sıralı arka plan adları içeriyor; kayıt biçimi `ad\0kaynak\0ff`. `CC.ALL` içindeki sabit `0x69` komutlarının ilk argümanı aynı sahnenin `CB.ALL` listesinde geçerli bir indeks: 44 kayıttaki 524 sabit komutun tamamı sınır içinde. `AL` sahnesinin altı komutu indeks 0–5 ile `ALGDoor1`, `ALGDoor2`, `ALKeyPad`, `ALGDoor3`, `ALNargRt`, `ALNDoor` adlarına eşleniyor. Bu bağlantı veri karşılaştırmasından çıkarımdır; çalışma sırası ve dinamik değerler henüz yorumlanmıyor. Toplam 376 adın 366'sı altı ISO'daki `backs` JPG yollarıyla eşleşiyor; beş `examine`, iki `exclip` özel kaydı ve üç ad (`ALGDoor2`, `BCCountr`, `EAPhne2`) eşleşmiyor. Eksik üç adın oyun akışındaki etkisi henüz bilinmiyor.

Akış: `FUN_0042d383`, `0xe1` etiketlerini kendilerinden sonraki komuta bağlıyor. `FUN_0042db22` içindeki `0x76` bu hedefe atlıyor; `0x9b`, `FUN_0043194f` ile dönüş adresini saklayıp hedefe gidiyor. `FUN_0042d8ba` dönüş komutları `0x6f` ve `0x83` için yığından geri dönüyor. Bu davranışın C++ karşılığı `byzantine-cc --flow` içinde çözümlendi. 44 betikte 7.693 benzersiz etiket, 74 yinelenen tanım ve 5.747 atlama/çağrı var; her hedef bulunuyor. Yinelenen etiketlerde özgün döngü son tanımı saklıyor.

`0x72` tıklama alanını başlangıçta kapalı kuruyor; `0x6d` ve `0x6e` komutları alanları açıp kapatıyor (`FUN_0042db22`, `FUN_0043340a`). `EA` betiğinde etiket 475, `EAEmShMs.jpg` görüntüsünü açtıktan sonra 76 ve 93 numaralı alanları etkinleştiriyor. [`scene_sample/EA-first-background.svg`](scene_sample/EA-first-background.svg) bu blok için tanısal örtüşüm. Görüntü 640×400, ekran tamponuna 20 satır aşağıdan yazılıyor (`FUN_0041045b` içindeki `+0x6400` bayt); SVG bu ofseti kullanıyor. Koşullu dallar ve daha önce etkinleştirilmiş alanlar bu örnekte yürütülmedi.

İlk sınırlı VM yürütümü `port/byzantine-cc --trace` ile eklendi. `EA` etiketi 475 için üç komut sonunda arka plan 0 (`EAEMShMs`) ve etkin alanlar 76/93 çıkıyor; `AL` etiketi 89 için arka plan 0 (`ALGDoor1`) ve alanlar 4/5 çıkıyor. Yalnızca `0xe1`, `0x72`, `0x69`, `0x6d`, `0x6e`, `0x76`, `0x9b`, `0x6f`, `0x83` ele alınıyor. Diğer işlem kodlarında izleme durur. Bu örnekler mevcut sahnenin tüm durumunu yeniden kurmaz.

## Sonraki teknik adım

BTB içindeki oyun akışı ve sahne/etkileşim verilerini belgeleyip taşınabilir motorun ilk oynanabilir kesitini kur. RLE çözücüyü farklı animasyonlarda ve özgün oyun görüntüsüyle daha geniş karşılaştırmada doğrula. Medya toplu restorasyonu, motorun varlık biçimi ve çözünürlük stratejisi netleşince yapılmalı.
