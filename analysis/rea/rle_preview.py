#!/usr/bin/env python3
"""Decode the game's 16-bit RLE animation frames into RGBA PNG previews."""

import argparse
import struct
import zlib
from pathlib import Path


def png_chunk(kind: bytes, data: bytes) -> bytes:
    body = kind + data
    return struct.pack(">I", len(data)) + body + struct.pack(">I", zlib.crc32(body))


def write_png(path: Path, width: int, height: int, rgba: bytes) -> None:
    rows = b"".join(b"\0" + rgba[y * width * 4 : (y + 1) * width * 4] for y in range(height))
    path.write_bytes(
        b"\x89PNG\r\n\x1a\n"
        + png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0))
        + png_chunk(b"IDAT", zlib.compress(rows, 9))
        + png_chunk(b"IEND", b"")
    )


def rgba555(word: int) -> bytes:
    r = (word >> 10) & 31
    g = (word >> 5) & 31
    b = word & 31
    return bytes(((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))


def decode_frame(words: tuple[int, ...], width: int, height: int) -> tuple[bytes, int]:
    pixels = bytearray(width * height * 4)
    i = 0
    x = 0
    y = -1
    painted = 0
    while i < len(words) and y < height:
        command = words[i]
        i += 1
        kind, count = command >> 14, command & 0x3FFF
        if kind == 1:
            y += 1
            x = count
            continue
        if kind == 3:
            x += count
            continue
        if kind == 0:
            if i + count > len(words):
                raise ValueError("Literal run exceeds frame data")
            run = words[i : i + count]
            i += count
        else:
            if i >= len(words):
                raise ValueError("Repeated run has no color")
            run = (words[i],) * count
            i += 1
        if y < 0 or y >= height or x + count > width:
            raise ValueError(f"Pixel run outside frame: x={x}, y={y}, count={count}")
        for color in run:
            offset = (y * width + x) * 4
            pixels[offset : offset + 4] = rgba555(color)
            x += 1
            painted += 1
    if y != height:
        raise ValueError(f"Frame ended after row {y}, expected {height} rows")
    return bytes(pixels), painted


def decode_file(source: Path, destination: Path) -> None:
    data = source.read_bytes()
    if len(data) < 10:
        raise ValueError("RLE file header is truncated")
    frame_count = struct.unpack_from("<H", data, 8)[0]
    offset = 10
    destination.mkdir(parents=True, exist_ok=True)
    for index in range(frame_count):
        if offset + 26 > len(data):
            raise ValueError(f"Frame {index} header is truncated")
        header = data[offset : offset + 26]
        width, height = struct.unpack_from("<HH", header, 2)
        word_count = struct.unpack_from("<I", header, 22)[0]
        offset += 26
        if not 0 < width <= 4096 or not 0 < height <= 4096:
            raise ValueError(f"Frame {index} has invalid size {width}x{height}")
        if offset + word_count * 2 > len(data):
            raise ValueError(f"Frame {index} data is truncated")
        words = struct.unpack_from(f"<{word_count}H", data, offset)
        offset += word_count * 2
        image, painted = decode_frame(words, width, height)
        output = destination / f"frame_{index:03d}.png"
        write_png(output, width, height, image)
        print(f"{output}: {width}x{height}, {painted} opaque pixels")
    if offset != len(data):
        print(f"Note: {len(data) - offset} trailing bytes remain")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    decode_file(args.source, args.destination)
