# Kaynak kodu rekonstrüksiyon durumu

En son adım: panorama düğümleri CO.ALL ile eşlendi; `0xba` yatay konum
komutu testli olarak eklendi. `0xb9` çizimi henüz uygulanmadı.
Güncel kısa kayıt: [CONTINUE.md](../../CONTINUE.md).
Kanıt ve kapsam: [PANORAMA_BINDINGS.md](PANORAMA_BINDINGS.md).

## Tam kaynak külliyatı (25 Ekim 2026)

`analysis/rea/reconstruction/` artık kayıtlı export'ların tamamını gezilebilir
biçimde içerir: 1.237 sözde-kod gövdesi (`functions/<adres>.c`), tam işlev
envanteri, 3.406 çağrı kenarı, 64 KB bölge haritası, çağrı-grafiği kümeleri ve
uygulanmamış gövdelerin yol haritası. Her export hash'i `decompile-index.json`
ile yeniden doğrulanır (1.237/1.237). Ayrıca `vm-commands.md` iki komut
dağıtıcısını (`0x43340a`: 99, `0x42db22`: 108) birleştirir: 155 opcode, 39
kurtarılmış mnemonic (GOTO, REPLY, HOOK, BACKGRND…), 28'i port VM'inde var.
Üreticiler: `analysis/rea/build_source_corpus.py` ve
`analysis/rea/build_vm_command_map.py`. Rapor: [SOURCE_CORPUS.md](SOURCE_CORPUS.md).
Bu külliye özgün kaynak değildir ve derlenmez; derlenebilir rekonstrüksiyon
`port/` olarak kalır.

## Gerçekçi sınır

`BETRAYAL.EXE` için özgün C/C++ proje dosyaları veya PDB sembol dosyası bu
çalışma alanında yok. Yürütülebilir dosya `betrayal.pdb` adına başvuruyor,
ancak dosyanın kendisi mevcut değil. Bu nedenle özgün kaynak kodun birebir
geri alınması mümkün değildir.

Bunun yerine iki ayrı çıktı korunur:

1. `pseudocode/functions/`: Ghidra'nın EXE'den ürettiği, adres-temelli
   sözde-kod. Bu dosyalar özgün isimleri, türleri veya derlenebilirliği garanti
   etmez.
2. `../../port/`: gözlenen dosya biçimleri ve davranışlardan sıfırdan yazılmış,
   derlenebilir C++17 taşınabilir rekonstrüksiyon. Özgün kodun kopyası değildir.

## Doğrulanan hedef

- Girdi: `BETRAYAL.EXE`
- SHA-256:
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
- Biçim: 32 bit x86 Windows PE
- Statik çözümleme: 1.397 işlev; 1.237 işlev için sözde-kod, 160 dış işlev

Sahne komut yorumlayıcısının ana gövdesi `FUN_0043340a` (`0x43340a`) olarak
tanımlandı. 25 Ekim 2026'daki REA incelemesi bu gövde için 369 temel blok ve
3.329 assembly satırı raporladı. Bu, statik kanıttır; oyunun çalıştırıldığı
anlamına gelmez. REA kanıtı: `ev_1c6d4d8619a7247425dead49cd7e2ea9cc162d494bbd6405a3a279467cfcd48c`.

## Derlenebilir rekonstrüksiyon

`port/` altında şunlar vardır:

- `byzantine-rle`: RGB555 RLE'yi RGBA/TGA karelerine çözer.
- `byzantine-all`: `.ALL` paketlerini listeler veya açar.
- `byzantine-cc`: `CC.ALL` sahne betiklerini ayrıştırır; sabit tıklama alanı,
  arka plan ve doğrulanmış çağrı/atlama akışını gösterir.

Bu makinede 25 Ekim 2026'da yeniden doğrulanan komutlar:

```sh
make -B -C port
port/byzantine-rle analysis/rea/HOTBAR.RLE /tmp/byzantine-rle-check
port/byzantine-cc port/output/cc_all/EA --trace 475 port/output/cb_all/EA
port/byzantine-cc port/output/cc_all/EA --flow
```

`EA --trace 475` arka plan `EAEMShMs` ile 76 ve 93 numaralı etkin alanları
verdi. Bu dar VM yolu; tüm oyun akışının çalıştığı iddiası değildir.

## Sonraki rekonstrüksiyon işi

Öncelik, `FUN_0043340a` içindeki henüz desteklenmeyen komutların anlamlarını
teker teker C++ yorumlayıcıya eklemektir. Özellikle koşullu dallar, durum
değişkenleri, konuşma/video/ses, envanter ve kayıt-yükleme davranışları
çözümlenmelidir. Her ekleme, EXE'den gelen statik kanıt ve ayrı bir testle
belgelenecektir.

`byzantine-cc --labels` tüm çözümlenmiş etiketleri dışarı verir. Bu yüzeyle
25 Ekim 2026'da 44 sahnede 7.693 etiket giriş yolu tarandı. Mevcut dar VM
1.458 yolu desteklenen dönüşe kadar götürebiliyor. `0x67` değişken ataması ve
`0x71`/`0x77` koşullu dalları artık sahne-içi olarak yürütülüyor; 1.099 yol,
önceki sahne durumundan gelen bilinmeyen bir değer nedeniyle açıkça duruyor.
Kalan yolların ilk bilinmeyen komutu en sık `0x90` (1.218), `0x64` (489) ve
`0x85` (396). Bu sayılar çalıştırılmış oyun davranışı değil, statik sahne
betikleri üzerindeki kontrollü yorumlayıcı kapsam ölçümüdür.

`0xb8` alan-geçiş komutu da eklendikten sonra aynı ölçüm 2.090 desteklenen yol
verdi. Bu komut, özgün motorun görünür alanları geçici tabloda işaretleyip
kapattığı `FUN_00438cf9` işlemini yansıtır; 25 Ekim 2026 devam adımında geçiş tablosu ve eşleşen `0xb7` geri açma
komutu da eklendi. Kaydı değiştirme, bir kez tüketme ve arada açılan alanları
koruma test edildi; bkz. [HOTSPOT_TRANSITIONS.md](HOTSPOT_TRANSITIONS.md).

## Debian sahne dikey kesiti

Durum aritmetiği `0x74`/`0x75` eklendi. Kaynak: kayıtlı REA/Ghidra
`pseudocode/functions/0x42db22.c` içindeki aynı işlem kodu dalları; değişken
operand çözümü `0x4319b3.c`. Testler 65535+2 sarmasını, değişkenden okunan
çıkarma miktarını, bilinmeyen hedefi ve çıkarma alt-taşma kontrolünü kapsıyor.

EA panorama araştırması: etiket 455 (komut 507–509) ortak temizleme etiketi
452'yi çağırıp değişken 5'i sıfırlıyor. Ancak başlangıç etiketi 475 bu kurulum
yolunu yürütmüyor. Bu bulgu varsayılan değer atamak için kullanılmadı;
panorama durumunu kuran gerçek giriş akışı ve `0xb9`/`0xba` komutları hâlâ
incelenmeli.

Oyuncuda tek CC betiği boyunca korunan `SceneState` eklendi: değişkenler ve
etkin alanlar her tıklamada sıfırlanmıyor. `make -C port test` hem yapay iki
giriş noktasında atama/koşul sürekliliğini hem özgün EA 475 → 595 geçişinde
76/93 alanlarının kapanıp 94'ün etkinleşmesini doğruluyor. 94'ün dönüş etiketi
602, artık uygulanan `0x9d` eşitlik çağrısında 5 numaralı panorama değişkeninin
değerini bekliyor. `0x9e` eşitsizlik çağrısı da eklendi; iki komut için koşulun
iki sonucunda dönüş adresi test edildi. Oyuncu tamamlanamayan tıklama eyleminin
kısmi durumunu uygulamaz ve durma nedenini gösterir. Bu yüzden bu örnek için
tam gidiş-dönüş oynanabilirlik henüz doğrulanmış değildir.

SDL2 ile `port/byzantine-play` eklendi. Prototip, bir dönüştürülmüş 640×400
BMP arka planını, ilgili CC/CB kayıtlarından gelen sahne iziyle birlikte
gösterir; etkin tıklama alanlarını çizer ve tıklanan alanın statik slot/action
bilgisini kaydeder. `action` alanı özgün tıklama yönlendiricisinde hedef etiket
tablosuna gittiği için prototip aynı etikete geçer ve hedef yeni bir arka plan
seçerse onu yükler. EA etiketi 475, `EAEMShMs` arka planı ile alan 76/93
üzerinde derlendi; alan 93'ün action 595 değeri `EAEmShCU` arka planına ulaştı.
 Bu, Debian'da gerçek oyun verisine bağlı ilk grafik dikey kesittir. Oyuncu
CD'den çıkarılan JPG arka planlarını doğrudan `ffmpeg` ile 640×400 yüzeye
çözüyor; sahneler arası durum, diyalog ve diğer komutlar henüz eksiktir.

## Sıralama koşulları — devam adımı

`0x96`–`0x99` ve `0x9f`–`0xa2` komutları eklendi; Debian derlemesi ve
144 karşılaştırma + 96 eksik değişken kontrolü geçti. Özgün EA 428 ve 464
etiketlerinin iki koşul sonucu da açıkça verilen durumla doğrulandı. Önceden
bu komutlarda duran 163 temiz durum izi artık gereken değişkeni bildiriyor;
yeni tamamlanmış yol sayısı arttığı iddia edilmiyor. Kanıt ve panorama
yükleyicisinin sınırları: [ORDERED_COMPARISONS.md](ORDERED_COMPARISONS.md).

## Özgün değişken eşleme tabloları

`SHOW.LCC`/`SHOW.LCV`/`SHOW.LVM` için C++ okuyucusu eklendi: 44 sahne,
1.622 yerel/genel eşleme, 1.049 genel ad doğrulandı. EA 5 = `lastNode`
(genel 8), EA 26 = `flowChk` (genel 74). Motorun sıfırlanan yeni oyun
havuzuna dayanan ayrı `--trace-new-game` seçeneğiyle 475 → 595 → 602 yolu
artık `0xb9` panorama komutuna ulaşıyor. Normal iz geçmiş durumu bilinmeyen
değişkenlerde durmaya devam ediyor. Başlangıç betiği, panorama çizimi ve
sahneler arası genel durum henüz tamamlanmadı. Derleme ve testler geçti;
ayrıntı: [VARIABLE_METADATA.md](VARIABLE_METADATA.md).
