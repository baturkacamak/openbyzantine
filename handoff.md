# Byzantine: video ve görsel restorasyonu devir notu

Son güncelleme: 25 Ekim 2026. Çalışma dizini artık `/media/baturkacamak/Backup/Byzantine - The Betrayal`. Önceki `~/Downloads/Byzantine - The Betrayal` yolu artık yok. Bu dosyadaki göreli yollar bu yeni köke göredir.

## Amaç ve mevcut durum

**Ana hedef:** Oyunu modern Windows, macOS ve Debian Linux'ta çalışacak şekilde yeniden düzenlemek/taşımak. Video ve görsel restorasyonu bu hedefin bir parçası; tek başına izlenebilir medya arşivi nihai ürün değil. Mevcut dizinde kaynak kod görünmüyor; altı ISO, ZIP arşivleri ve restorasyon örnekleri var. CD1'deki `copy2hd/BETRAYAL.EXE`, 32 bit Windows GUI uygulaması ve `WINMM.dll`, `AVIFIL32.dll`, `MSVFW32.dll`, `DDRAW.dll`, `DSOUND.dll` kullanıyor. Bu nedenle önce kaynak kodun başka yerde bulunup bulunmadığı ve oyun motoru/veri biçimlerinin nasıl taşınacağı belirlenmeli.

Kullanıcı, oyunun tüm video ve görsellerini yerel yapay zekâ araçlarıyla iyileştirmek de istiyor. Görüntü tercihi, açılış videosunda hazırlanan **dengeli 30 fps** sürümü. 60 fps sürümü beğenilmedi; toplu işlem için 60 fps kullanılmamalı.

- Tamamlanan ve seçilen örnek: [`upscale_ornek/mintro_dengeli_30fps.mp4`](upscale_ornek/mintro_dengeli_30fps.mp4). 640×400, 30 fps, 6.216 kare, 207,2 saniye, H.264 + AAC. Kaynak ses korunarak kopyalandı.
- 15 fps temel restorasyon: [`upscale_ornek/mintro_dengeli.mp4`](upscale_ornek/mintro_dengeli.mp4). 640×400, 3.108 kare, 207,2 saniye.
- Dört yönlü karşılaştırma: [`upscale_ornek/karsilastirma_4lu_20s.mp4`](upscale_ornek/karsilastirma_4lu_20s.mp4). Sol üst kaynak, sağ üst Upscayl-lite, sol alt RealBasicVSR, sağ alt dengeli sürüm.
- Alternatif tam sürümler: `mintro_dogal.mp4`, `mintro_realbasicvsr.mp4`, `mintro_upscaled.mp4`. `mintro_dengeli_60fps.mp4` dosyası da mevcut ama kullanıcı bu sürümü seçmedi.
- Kaynak ISO'lar, ZIP arşivleri ve oyun dosyaları değiştirilmedi. Çıktılar şu an yalnızca bağımsız izlenebilir dosyalar; oyun içinde kullanılabilirlik henüz sınanmadı.
- [`upscale_ornek/README.txt`](upscale_ornek/README.txt) 30 fps tercihi yapılmadan önce yazılmıştır; bu devir notu tercih konusunda günceldir.

## Ortam

- GPU: NVIDIA GeForce GTX 1660 Ti, 6 GB VRAM. RealBasicVSR 30 karelik gruplar ve iki yanda 5 kare bağlamla çalıştı. Daha büyük gruplar VRAM'i aşabilir.
- Yerel Python: `/home/baturkacamak/.local/opt/upscale-tools/ComfyUI/.venv/bin/python`; PyTorch ve CUDA kurulu.
- Model: `/home/baturkacamak/.cache/byzantine-vsr/realbasicvsr.pth` (142 MB). [Resmî OpenMMLab ağırlığı](https://download.openmmlab.com/mmediting/restorers/real_basicvsr/realbasicvsr_c64b20_1x30x8_lr5e-5_150k_reds_20211104-52f77c2c.pth).
- Bağımsız çıkarım betiği: [`upscale_ornek/realbasicvsr_test.py`](upscale_ornek/realbasicvsr_test.py). OpenMMLab BasicVSR/RealBasicVSR mimarisinin PyTorch uygulaması; eski MMEditing/MMCV kurulumuna ihtiyaç duymaz. Betik tüm videoyu önce belleğe okur, ardından kare gruplarını GPU'da işler; uzun videolar için RAM ve yeniden başlatılabilirlik yönünden iyileştirilmelidir.
- Görsel aracı: `upscale-image` (varsayılan `high-fidelity-4x` modeliyle 2×). JPG/PNG kabul eder; TGA ve oyun RLE dosyalarını doğrudan kabul etmez.
- Taşınan dizinin bulunduğu Backup bölümünde son kontrolde yaklaşık **601 GB boş alan** vardı. Önceki 5,3 GB boş alan sorunu eski sistem bölümüne aitti; yeni bölüm için geçerli değildir. Model önbelleği hâlâ ev dizininde olduğu için geçici dosyaların hangi bölümde üretildiğine dikkat edilmeli.

## Dengeli 30 fps yöntemi: tam yeniden üretim

Kaynak dosya CD1 içindeki `video/intro/mintro.avi`: 320×200, 15 fps, 3.108 kare, 207,2 saniye, Indeo 4 video + PCM ses. Çıkarılmış kopyası `upscale_ornek/mintro.avi`.

1. **RealBasicVSR:** Kaynağı 4× modelle işleyip betikte 640×400'e indir. Kullanılan komut:

   ```bash
   cd '/media/baturkacamak/Backup/Byzantine - The Betrayal'
   /home/baturkacamak/.local/opt/upscale-tools/ComfyUI/.venv/bin/python \
     upscale_ornek/realbasicvsr_test.py \
     upscale_ornek/mintro.avi \
     upscale_ornek/mintro_realbasicvsr.mp4 \
     --checkpoint /home/baturkacamak/.cache/byzantine-vsr/realbasicvsr.pth \
     --chunk 30 --overlap 5
   ```

2. **Doğal referans:** Kaynağı çok hafif temizle, 640×400'e ölçekle, hafif keskinleştir. Amaç modelin fazla düzleştirdiği yerlerde kaynak dokusunu korumak:

   ```bash
   ffmpeg -y -i upscale_ornek/mintro.avi \
     -vf 'hqdn3d=0.8:0.6:1.2:0.9,scale=640:400:flags=spline,unsharp=3:3:0.25:3:3:0' \
     -c:v libx264 -crf 16 -preset medium -pix_fmt yuv420p \
     -c:a aac -b:a 192k -movflags +faststart upscale_ornek/mintro_dogal.mp4
   ```

3. **Dengeli harman:** Her karede RealBasicVSR %55, doğal referans %45. Ses doğrudan asıl AVI'den alındı:

   ```bash
   ffmpeg -y -i upscale_ornek/mintro_realbasicvsr.mp4 \
     -i upscale_ornek/mintro_dogal.mp4 -i upscale_ornek/mintro.avi \
     -filter_complex "[0:v][1:v]blend=all_expr='A*0.55+B*0.45'[v]" \
     -map '[v]' -map 2:a:0 -c:v libx264 -crf 16 -preset slow \
     -pix_fmt yuv420p -c:a aac -b:a 192k -shortest \
     -movflags +faststart upscale_ornek/mintro_dengeli.mp4
   ```

4. **30 fps:** FFmpeg `minterpolate` ile hareket tahmininden ara kare üretildi. Sondaki karelerin kesilmesini önlemek için `tpad` ve tam süreli `trim` kullanıldı. `207.2` yalnızca bu açılış videosunun süresidir; diğer dosyalarda `ffprobe` ile gerçek süreyi alıp komuta verin.

   ```bash
   ffmpeg -y -nostdin -i upscale_ornek/mintro_dengeli.mp4 \
     -vf 'tpad=stop_mode=clone:stop_duration=0.2,minterpolate=fps=30:mi_mode=mci:mc_mode=aobmc:me_mode=bidir:vsbmc=1:scd=fdiff,trim=duration=207.2,setpts=PTS-STARTPTS' \
     -c:v libx264 -crf 16 -preset medium -pix_fmt yuv420p \
     -c:a copy -movflags +faststart upscale_ornek/mintro_dengeli_30fps.mp4
   ```

Bu yöntemde 30 fps, kaynakta olmayan ara kareleri tahmin eder. Hızlı hareket, örtüşen nesneler ve sahne geçişlerinde bozulma veya hayaletlenme olabilir. Kullanıcı açılış videosunun genel sonucunu beğendi. Diğer videolarda sorunlu sahneler ayrıca incelenmeli; 60 fps otomatik uygulanmamalı.

## Altı ISO'nun envanteri

Altı diskin toplamında 8.846 dosya var. Aşağıdaki **ayrı dosya** sayıları, küçük/büyük harfe duyarsız iç yol + dosya boyutuna göre tekilleştirildi; gerçek içerik özdeşliği için toplu çalışmadan önce SHA-256 doğrulaması yapılmalı. Aynı dosyaların birçok CD'de tekrar etmesi nedeniyle her disk için yeniden işlem yapmak gereksiz.

| Tür | ISO içindeki kayıt | Yaklaşık ayrı dosya | Ölçülen özellik |
| --- | ---: | ---: | --- |
| AVI | 971 | 592 | Toplam 5.365,8 sn = **89,4 dk**, 80.710 kaynak kare. 549'u 320×200; çoğu 15 fps ve Indeo 4. `dialog/` klipleri 364 ayrı dosya. |
| JPG | 2.121 | 599 | 473 adet 640×400; 116 adet 2496×768 panorama; 9 adet 640×480; 1 küçük görsel. |
| TGA | 1.434 | 314 | Çoğu küçük arayüz/ikon; 308'inde saydamlık var. |
| RLE | 2.201 | 947 | Oyuna özgü animasyon/varlık biçimi. FFmpeg ve standart görsel okuyucular açamıyor. |
| WAV | 1.642 | 474 | Ses ayrı bir restorasyon konusu; upscale edilmez. |

AVI'lerin 80'i 2 saniye veya daha kısa, 341'i 2–10 saniye, 156'sı 10–30 saniye, 15'i 30 saniyeden uzun. En uzun iki sahne `video/intro/mintro.avi` (207,2 sn) ve `video/intro/MEPILOG.AVI` (134,7 sn). Bazı AVI'ler 320×200 değil: portre/küçük animasyon boyutları ve birkaç 640×400 dosya var. **Tüm videoları körlemesine 640×400'e ölçeklemeyin**; her dosyanın en-boy oranı ve kullanım amacı belirlenmeli.

## Toplu iş için önerilen sıra

### 1. Önce oyuna geri koyma sınaması

Hedef iki ayrı çıktı olabilir: (a) dışarıdan izlenebilen restore arşivi, (b) oyunda çalışan değiştirilmiş varlıklar. (a) için mevcut MP4/JPG çıktıları yeterli. (b) için önce küçük ve geri alınabilir bir uçtan uca sınama gerekli:

1. Orijinal ISO/ZIP dosyalarının SHA-256 özetlerini kaydet; orijinalleri salt okunur kabul et.
2. Oyun ortamını/kurulumunu belirle. Tek bir AVI, tek bir JPG ve saydam tek bir TGA ile oyun içinde dene. Hangi dosyanın CD'den, hangisinin kurulu klasörden okunduğunu kontrol et.
3. Video için kapsayıcı, codec, kare hızı ve çözünürlük kabulünü ölç. Mevcut orijinal videoların çoğu Indeo 4 AVI; hazırlanan H.264 MP4'lerin oyunda doğrudan açılacağı doğrulanmadı. Buradaki FFmpeg kurulumunda Indeo 4 **kodlayıcısı yok**. 30 fps ve 640×400 kabulü ayrıca sınanmalı.
4. JPG/TGA'da oyun koordinatlarının sabit boyuta bağlı olup olmadığını kontrol et. 640×400 arka planı 1280×800 yapmak tek başına oyunun onu yüksek çözünürlükte göstereceği anlamına gelmez; panoramalarda kırpma/gezinti, TGA'da alfa kanalı korunmalı.
5. Bu küçük sınamalar geçmeden tüm varlıkları işleyip ISO'ları yeniden paketleme.

### 2. Yeniden üretilebilir envanter ve iş kuyruğu

- Her ISO için `7z l -slt` çıktısından disk, özgün yol, boyut ve uzantı manifesti üret. `ffprobe` ile her AVI'nin süre, kare sayısı, FPS, codec, ses ve boyutunu kaydet. JPG/TGA boyutlarını Pillow ile oku.
- Aynı yol+boyut eşleşmelerini SHA-256 ile doğrula. Aynı içerik bir kez işlenip ilgili CD çıktılarına eşlenebilir. Aynı yol fakat farklı içerik/ölçü mutlaka ayrı tutulmalı.
- Manifestte durum alanları olsun: `bekliyor`, `işleniyor`, `tamam`, `doğrulandı`, `sorunlu`. Çıktıyı önce geçici ada yaz, `ffprobe` ve decode kontrolünden sonra atomik olarak son adına taşı. Kesintide yalnızca eksik dosyalar işlensin.
- Geçici kareleri Backup bölümünde üret; tüm kareleri aynı anda PNG'ye açma. Disk kullanımını ve VRAM'i izleyerek dosya bazında ilerle.

### 3. AVI videoları

- Önce 2–3 farklı klip türünde 15–20 saniyelik örnekler hazırla: yüzlü diyalog, hızlı hareket, sahne geçişi, grafik/animasyon. Kullanıcının seçtiği dengeli 30 fps görünümü karşılaştırmalı olarak doğrula.
- 320×200 canlı görüntü için yukarıdaki RealBasicVSR → doğal referans → %55/%45 harman → 30 fps zincirini temel al. Oranı ve hafif temizleme düzeyini, orijinaldeki yazı/yüz bozulmalarına göre klip bazında ayarlamaya izin ver.
- Kısa kliplerde 30 kare + 5 kare örtüşme yerine sahne kesimlerini algılayan, sınırda birkaç kare bağlam bırakan bir işleyici yaz. `realbasicvsr_test.py` bütün videoyu RAM'e yüklediği ve ara durum kaydetmediği için 592 dosyalık kuyrukta doğrudan kullanmak yerine akış ve yeniden başlatma desteği ekle.
- Model girişinde özgün en-boy oranını koru. 320×200 → 640×400. Farklı boyuttaki ve saydamlık içerebilecek oyun animasyonlarını ayrı kategoriye koy. 640×400 kaynakları otomatik 2× büyütmek zorunlu değil.
- Orijinal sesten yeni MP4'e ses aktar; süreyi ve kanal düzenini doğrula. Ses olmayan videolarda `-map 1:a?` benzeri isteğe bağlı ses eşleme kullan.
- Kalite kontrol: ilk/orta/son kare, sahne geçişi, ses eşzamanı, tam süre, beklenen kare sayısı (`süre × 30`), bozuk kare ve dosyayı baştan sona FFmpeg ile çözme. Rastgele örneklerle kullanıcıya kıyaslama imkânı ver.

### 4. JPG ve TGA görseller

- 599 JPG'yi kullanımına göre ayır: 640×400 arka plan/envanter ve 2496×768 panorama. Yüz, yazı ve menü öğelerinde yapay ayrıntı hatalarına bak. `upscale-image` 2× yüksek doğruluk modeliyle JPG/PNG işliyor; dosyaları özgün hâllerinden üret.
- 314 TGA'nın çoğu saydam arayüz görseli. RGBA olarak PNG'ye dönüştür, yalnızca renk kanallarını uygun modelle büyüt, alfa/maskeyi ayrı ölçekle ve yeniden birleştir. Kenarlarda halo ve yanlış saydamlık olup olmadığını kontrol et. Eğer oyun TGA istiyorsa bit derinliği ve alfa düzeni korunarak geri yazılması gerekir.
- Panoramalarda 2× çıktı yaklaşık 4992×1536 olur. Oyun motoru boyutu kabul etmiyorsa ayrıca özgün ölçüde temizlenmiş sürüm gerekebilir.

### 5. RLE ve diğer varlıklar

- 947 RLE dosyası standart RLE görüntüsü gibi okunmuyor; bir kısmı animasyon dizisi, bir kısmı tam ekran/küçük nesne olabilir. Başlık alanlarında farklı sürümler ve kare sayısına benzeyen değerler görüldü fakat biçim henüz çözülmedi. Sayılarını “947 resim” veya “947 video” diye varsaymayın.
- Önce oyun yürütülebilir dosyası ve veri erişimi üzerinden RLE kod çözücüyü belgelemek, kareleri ve alfa/paleti doğru çıkarmak, sonra **özgün biçime geri kodlamak** gerekir. Bir örneği oyunda doğrulamadan toplu işlem yapılmamalı.
- WAV, font, `BTB`, betik ve yürütülebilir dosyalar ayrı envanterde tutulmalı. Ses için görüntü upscale modeli kullanılmaz; gerekiyorsa gürültü/ses seviyesi restorasyonu ayrıca planlanır.

## Süre, depolama ve karar noktaları

- Ölçülen örnek: açılış videosunun RealBasicVSR işlemi yaklaşık 14 dakika; 30 fps dönüştürmesi yaklaşık 10 dakika. Bu hızla **592 ayrı AVI için yaklaşık 10–12 saat** hesaplanıyor. 80 kısa klip ve farklı çözünürlükler yüzünden gerçek süre değişir.
- `upscale-image` ile örnek 640×400 JPG'nin 2× işlemi **21,7 sn**, 2496×768 panoramanın 2× işlemi **29,2 sn** sürdü. JPG+TGA grubu, alfa işlemleri dâhil yaklaşık **4–6 saat**. Böylece RLE ve oyun entegrasyonu hariç kabaca **15–20 saat** makine işlemi beklenir.
- RLE biçiminin çözülmesi, oyun uyumluluğu, tekrar kodlama ve manuel kalite kontrol için güvenilir süre tahmini henüz yok; bu iş makine süresinden çok araştırma/geliştirme süresidir.
- Yeni Backup bölümünde yeterli boş alan var; yine de tam çıktı, ara dosyalar ve olası yeni ISO'lar için en az 10–20 GB boş alan rezervi ve düzenli temizlik yapılmalı. Orijinal ZIP/ISO dosyalarını sonuçlar doğrulanana kadar silme veya üzerine yazma.

## İlk yapılacak somut iş

Önce kaynak kodun erişilebilir olup olmadığını kullanıcıyla netleştir. Varsa kod tabanını ve bağımlılıklarını inceleyip Windows, macOS ve Debian için taşınabilir derleme yolunu kur. Yoksa özgün yürütülebilir dosya, veri dosyaları ve oyun akışını belgeleyerek uyumlu yeni motor/yeniden uygulama kapsamını belirle; güncel ScummVM desteğini de doğrula. Sonrasında en küçük oynanabilir kesiti üç platformda çalıştırmayı hedefle. AVI/JPG/TGA/RLE ve restore edilmiş varlıkların kabulü bu motor çalışmasının içinde test edilmeli. Tam medya toplu işlemine, yeni motorun biçim/çözünürlük stratejisi netleşince geç.

## 25 Ekim 2026: REA kurulumu ve ilk analiz

Kullanıcının istediği [REA](https://github.com/morluto/rea) aracı `tools/rea` içine klonlandı, Node.js 24.22.1 ile derlendi ve Codex MCP bağlantısı kuruldu. Java 21 ile Ghidra 12.1.4 ayrıca kuruldu. CLI ile `BETRAYAL.EXE` üzerinde statik analiz başarıyla çalıştı. Ayrıntılar ve kanıt dosyaları [`analysis/rea/README.md`](analysis/rea/README.md) içinde. Codex'in REA MCP araçlarını görmesi için uygulama/oturumun yeniden başlatılması gerekiyor; CLI şimdi kullanılabilir.

Sonraki oturumda REA MCP araçları açıldı ve doğrudan kullanıldı. `rea.sh`, `sh rea.sh` ile de çalışacak şekilde düzeltildi; varsayılan analiz bu biçimde başarıyla çalıştırıldı. Oyunun gerçek RLE çizim kodundan [`analysis/rea/rle_preview.py`](analysis/rea/rle_preview.py) geliştirildi; `HOTBAR.RLE` ve `EAKEY.RLE` örnekleri saydam PNG olarak çözüldü. İlkinde Türkçe YÜKLE/KAYDET düğmesi, ikincisinde anahtar görseli çıktı. Yeni kanıtlar `analysis/rea/mcp-session.json` dosyasında; ayrıntılar `analysis/rea/README.md` içinde. Bu bir veri biçimi prototipi, henüz oynanabilir taşınabilir motor değil.

Kullanıcı kaynak kodu olmadığını doğruladı ve REA ile yeniden kaynak kod yaratılmasını istedi. [`port/`](port/) altında C++17 taşınabilir kaynak kod tabanı başlatıldı. `byzantine-rle` ve `byzantine-all` Debian'da derlendi ve çalıştı. RLE'nin altı örnek karesinin pikselleri Python prototipiyle birebir eşleşti. CD1'deki 19 `.ALL` arşivinin tamamı çözücüden geçti. `.ALL` biçimi REA ile incelenen `FUN_00411b4b` yükleyicisinden çıkarıldı. `Byz.all` Türkçe metin içeriyor; `SHOW.LVM` içinde değişken/işlev adları görünüyor. Sıradaki ana iş bu oyun akışı/VM biçimini ve sahne motorunu çözmek. Ayrıntılar [`port/README.md`](port/README.md) içinde. Windows ve macOS derlemeleri henüz doğrulanmadı; oyun hâlâ oynanabilir değil.

Ayrıca `byzantine-cc` C++17 ayrıştırıcısı yazıldı. REA ile `FUN_00412762` komut okuyucusu ve 0x32–0xE1 işlem kodu argüman sayısı tablosu çıkarıldı. `CC.ALL` içindeki **44 sahne betiğinin tümü** dosya sonuna kadar hatasız ayrıştırıldı. Bu, yalnızca komutların yapısını okuyor; işlem kodlarının anlamı ve oyun yürütümü henüz uygulanmadı. Güncel kanıt `analysis/rea/mcp-session.json` içinde, kaynak kod ve sınırları `port/README.md` içinde.

REA toplu sözde kod çıkarımı tamamlandı: 1.397 tanınan işlevin 1.237'si `analysis/rea/pseudocode/functions/` içinde, 160'ı dış işlev olarak listelendi. `port/byzantine-cc --hotspots`, `FUN_0042db22` içindeki `0x72` komutunun sabit tıklama alanlarını çıkarıyor. Bu oyunun sahne etkileşimine doğru somut ilerleme; dinamik operandlar, koşullar, yürütme sırası ve kalan işlem kodu davranışları henüz çözülmedi.

Son kontrolde CD1'in 44 `CC.ALL` betiğinden 2.070 sabit tıklama alanı tanımı çıkarıldı. Tahmin: tek geliştirici tam zamanlı çalışırsa ilk oynanabilir kesit için birkaç hafta/1–2 ay, tüm oyunun üç platformda güvenilir çalışması için en az birkaç ay; belirsizlik en çok işlem kodlarının davranışı, çoklu CD akışı, eski video/ses ve uçtan uca oyun testlerinde. Bu bir taahhüt değil; iş kalemi ve gerçek oynanabilirlik ölçümleri ilerledikçe daraltılmalı.

Bir sonraki incelemede arka plan yolu çözüldü: `0x69` sahne komutu `FUN_00431ec2` üzerinden `FUN_0041045b` yükleyicisine gidiyor. Düğüm kaydından `backs` altındaki JPG adı kuruluyor; `FUN_0040f135` 640×400 JPEG'i 16 bit görüntü tamponuna çözüyor. CD1 ISO'sunda `backs` altında 215 JPG sayıldı ve örnek `backs/bc/BCAKALIN.JPG` dosyasının 640×400 olduğu doğrulandı. `port/CMakeLists.txt` ile üç araç için C++17 CMake derleme iskeleti eklendi; CMake bu makinede kurulu olmadığından henüz çalıştırılmadı. Ayrıntı `analysis/rea/README.md` ve `port/README.md` içinde.

`CB.ALL` arşivinin sahne başına arka plan adı listeleri bulundu ve C++17 `port/src/cb.cpp` ayrıştırıcısı yazıldı. `byzantine-cc <CC kayıt> --backgrounds <CB kayıt>` artık `0x69` komut numaralarını JPG adlarıyla eşliyor. 44 sahne, 376 ad ve 524 sabit arka plan komutu başarıyla kontrol edildi. Altı ISO'daki JPG listesiyle 366 ad eşleşiyor; 10 kayıt özel veya bulunamıyor. Kod `make -B -C port` ile derlendi. Ayrıntı `port/README.md` ve `analysis/rea/README.md` içinde.

Sonraki adımda sahne akışı çözüldü: `0xe1` etiket, `0x76` atlama, `0x9b` çağrı, `0x6f`/`0x83` dönüş. `port/byzantine-cc --flow` 44 betikteki 5.747 atlama/çağrının tüm hedeflerini çözüyor; 7.693 benzersiz etiket ve 74 yinelenen tanım var. `0x72` alanları kapalı başlatıyor, `0x6d` açıyor, `0x6e` kapatıyor. `analysis/rea/scene_sample/EA-first-background.svg`, `EAEmShMs.jpg` ve aynı kod bloğunda etkinleşen iki alan için ilk görsel tanısal örnek. Bu henüz oyunu çalıştıran yorumlayıcı değil; koşullu akış, medya ve platform GUI katmanı eksik.

`port/byzantine-cc --trace <etiket> <CB kayıt>` komutuyla sınırlı VM yürütümü eklendi. `EA --trace 475` üç komutta arka plan 0 ve 76/93 alanlarını, `AL --trace 89` arka plan 0 ve 4/5 alanlarını üretti; ikisi de dönüşte durdu. Bilinmeyen işlem kodları atlanmıyor, durma nedeni gösteriliyor. `make -B -C port` ve 44 betikte `--flow` doğrulaması başarılı.
