**TÜRKÇE (TR)**
Kısaca

KOR (.kr) Türkçe anahtar kelimelerle yazılmış, sistem-odaklı bir scripting/DSL dilidir. korc adında tek dosya C tabanlı derleyici sağlar: .kr → (RAM içinde) C üretilir → gcc ile derlenir → çalıştırılır. Eğitim, otomasyon ve basit sistem servisleri için tasarlandı.

Özet

Dosya uzantısı: .kr

Derleyici: korc

Hedef platform: POSIX (Linux, macOS benzeri)

Sürüm: v1.1

Lisans: MIT

Özellikler

Türkçe anahtar kelimeler: eger, yaz, bekle, kapat, port ac, html_baslat vb.

Tek binary: kullanıcı gcc bilmek zorunda değil

Basit HTTP sunucu (localhost), TUI (ncurses varsa) veya terminal arayüzü

Türkçe hata mesajları, exit kodları

Root gerektiren komutlarda geteuid() kontrolü

V1 hedefi: öğretici, hafif, çalışır halde bir çekirdek

Kurulum & Derleme

Gereksinimler:

gcc (C11)

POSIX ortam

(opsiyonel) libncurses-dev — TUI için

Debug / geliştirme:

gcc -std=c11 -O0 -g korc.c -o korc


Release (statik, taşınabilir):

gcc -std=c11 -O2 -static -fno-pie -no-pie korc.c -o korc


ncurses ile:

gcc -std=c11 -O2 -static -DHAVE_NCURSES korc.c -lncurses -o korc

Kullanım
./korc <komut> [secenekler] <dosya.kr>

Komutlar:
  check   Syntax ve semantic kontrol
  build   Binary olustur (varsayilan: input ismi uzantisiz)
  run     Derle ve calistir (gecici binary)
  tui     TUI arayuzu (ncurses varsa)

Secenekler:
  -o <dosya>   Build ile cikti adi belirle
  -v           Versiyon
  -h           Yardim


Örnek:

./korc check examples/hello.kr
./korc build examples/hello.kr -o hello
././hello
./korc run examples/web.kr
./korc tui examples/web.kr

Dil Özeti (Kısa)

Anahtar kelimeler (örnek):
eger, ise, degilse, son, tekrar, dongu, fonksiyon, don, yaz, yaz_satir, oku, bekle, milisaniye_bekle, simdi, dosya_ac, dosya_yaz, dosya_oku, dosya_kapat, dosya_varmi, port, html_baslat, html_bitir, sunucu_baslat, calistir, kapat, yeniden_baslat, cpu, cpu_sicaklik, ram, disk, sadece_root, cik

Yorum:

Blok yorum: @. … .@

Basit örnek — Merhaba Dünya (examples/hello.kr):

@. Merhaba Dünya örneği .@
yaz "Merhaba KOR"
yaz_satir "KOR'a hoş geldiniz"


Basit web sunucusu (examples/web.kr):

#kullan net.h
#kullan stdio.h

yaz "Web sunucu hazırlanıyor"
port 8080 ac

html_baslat
<html><body><h1>Merhaba KOR</h1></body></html>
html_bitir

sunucu_baslat

Hata Kodları & Mesajlar

Exit kodları:

0 — Başarı

1 — Genel hata

2 — Syntax hata

3 — Semantic / namespace hata

4 — Runtime hata

5 — Derleme (gcc) hatası

6 — Güvenlik / izin hatası

Hata formatı:

HATA E### (satır N):
<Türkçe açıklama>
Öneri: <kısa öneri>

Bilinen Hata (v1.1)

BILINEN HATA: SIGABRT (invalid free / heap corruption) — bazı koşullarda free() çağrılarından dolayı abort oluşabiliyor.

Geliştirme sırasında valgrind veya -fsanitize=address ile inceleyin.

Release kullanımında çöküşleri sessiz kapatma tercih edildi (signal handler).

Güvenlik

calistir komutu whitelist ile sınırlandırılmıştır; sandbox önerilir.

HTTP sunucu v1’de sadece localhost.

Katkıda Bulunma

Fork → feature branch → commit → PR

Testler: tests/ dizinindeki scriptleri çalıştırın (varsa)

V1.1 sonrası öneriler: bellek ownership düzeltmeleri, TUI iyileştirme, websocket/SSL modülleri.

Lisans

MIT © 2025 — IOSMEN

**ENGLISH (EN)**
Overview

KOR (.kr) is a Turkish-syntax, system-focused scripting/DSL. The repo provides a single-file C-based compiler korc: .kr → (in-memory) C code → compiled with gcc → executable. Designed for teaching, automation and simple system services.

Summary

File extension: .kr

Compiler: korc

Target: POSIX systems (Linux/macOS-like)

Version: v1.1

License: MIT

Features

Native Turkish keywords: eger (if), yaz (print), bekle (sleep), kapat (shutdown), port ac, html_baslat, etc.

Single binary korc — users need not know gcc or C

Minimal HTTP server (localhost), TUI (if ncurses available) or terminal UI

Turkish error messages and exit codes

Security checks for root-only commands using geteuid()

V1 goal: lightweight, educational, working core

Install & Build

Requirements:

gcc with C11 support

POSIX environment

(optional) libncurses-dev for TUI

Debug / dev:

gcc -std=c11 -O0 -g korc.c -o korc


Release (static portable):

gcc -std=c11 -O2 -static -fno-pie -no-pie korc.c -o korc


With ncurses:

gcc -std=c11 -O2 -static -DHAVE_NCURSES korc.c -lncurses -o korc

Usage
./korc <command> [options] <file.kr>

Commands:
  check   Syntax and semantic check
  build   Produce binary (default: input name without extension)
  run     Compile and run (temporary binary)
  tui     Start TUI (if ncurses available)

Options:
  -o <file>   Output file name for build
  -v          Version
  -h          Help


Examples:

./korc check examples/hello.kr
./korc build examples/hello.kr -o hello
././hello
./korc run examples/web.kr
./korc tui examples/web.kr

Language Quick Reference

Keywords (sample):
eger, ise, degilse, son, tekrar, dongu, fonksiyon, don, yaz, yaz_satir, oku, bekle, milisaniye_bekle, simdi, dosya_ac, dosya_yaz, dosya_oku, dosya_kapat, dosya_varmi, port, html_baslat, html_bitir, sunucu_baslat, calistir, kapat, yeniden_baslat, cpu, cpu_sicaklik, ram, disk, sadece_root, cik

Comments:

Block comments: @. … .@

Example — Hello World (examples/hello.kr):

@. Hello World example .@
yaz "Merhaba KOR"
yaz_satir "Welcome to KOR"


Example — Simple Web (examples/web.kr):

#kullan net.h
#kullan stdio.h

yaz "Starting web server"
port 8080 ac

html_baslat
<html><body><h1>Merhaba KOR</h1></body></html>
html_bitir

sunucu_baslat

Error Codes & Messages

Exit codes:

0 — Success

1 — General error

2 — Syntax error

3 — Semantic / namespace error

4 — Runtime error

5 — Compilation (gcc) error

6 — Security / permission error

Message format:

HATA E### (satır N):
<message in Turkish>
Öneri: <short suggestion>

Known Issue (v1.1)

KNOWN BUG: SIGABRT (invalid free / heap corruption) — in some conditions free() causes abort.

Debug with valgrind or -fsanitize=address.

Release defaults to silent crash handling (signal handler).

Security

calistir is guarded by a simple whitelist. Use sandboxing for production.

HTTP server runs on localhost only in v1.

Contributing

Fork → feature branch → PR

Tests: run scripts in tests/ (if present)

Suggested v1.1 improvements: memory ownership fixes, improved TUI, websocket/SSL modules.

License

MIT © 2025 — IOSMEN
