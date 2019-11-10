#ifndef FOUNDATION_HPP
#define FOUNDATION_HPP

#include <iostream>
#include <cstdint>
using namespace std;

typedef int8_t   Int8;
typedef int16_t  Int16;
typedef int32_t  Int32;
typedef int64_t  Int64;
typedef uint8_t  UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

#define FILE_A 0x0101010101010101
#define FILE_B 0x0202020202020202
#define FILE_C 0x0404040404040404
#define FILE_D 0x0808080808080808
#define FILE_E 0x1010101010101010
#define FILE_F 0x2020202020202020
#define FILE_G 0x4040404040404040
#define FILE_H 0x8080808080808080

#define RANK_1 0x00000000000000FF
#define RANK_2 0x000000000000FF00
#define RANK_3 0x0000000000FF0000
#define RANK_4 0x00000000FF000000
#define RANK_5 0x000000FF00000000
#define RANK_6 0x0000FF0000000000
#define RANK_7 0x00FF000000000000
#define RANK_8 0xFF00000000000000

#define a1 0x1
#define b1 0x2
#define c1 0x4
#define d1 0x8
#define e1 0x10
#define f1 0x20
#define g1 0x40
#define h1 0x80
#define a2 0x100
#define b2 0x200
#define c2 0x400
#define d2 0x800
#define e2 0x1000
#define f2 0x2000
#define g2 0x4000
#define h2 0x8000
#define a3 0x10000
#define b3 0x20000
#define c3 0x40000
#define d3 0x80000
#define e3 0x100000
#define f3 0x200000
#define g3 0x400000
#define h3 0x800000
#define a4 0x1000000
#define b4 0x2000000
#define c4 0x4000000
#define d4 0x8000000
#define e4 0x10000000
#define f4 0x20000000
#define g4 0x40000000
#define h4 0x80000000
#define a5 0x100000000
#define b5 0x200000000
#define c5 0x400000000
#define d5 0x800000000
#define e5 0x1000000000
#define f5 0x2000000000
#define g5 0x4000000000
#define h5 0x8000000000
#define a6 0x10000000000
#define b6 0x20000000000
#define c6 0x40000000000
#define d6 0x80000000000
#define e6 0x100000000000
#define f6 0x200000000000
#define g6 0x400000000000
#define h6 0x800000000000
#define a7 0x1000000000000
#define b7 0x2000000000000
#define c7 0x4000000000000
#define d7 0x8000000000000
#define e7 0x10000000000000
#define f7 0x20000000000000
#define g7 0x40000000000000
#define h7 0x80000000000000
#define a8 0x100000000000000
#define b8 0x200000000000000
#define c8 0x400000000000000
#define d8 0x800000000000000
#define e8 0x1000000000000000
#define f8 0x2000000000000000
#define g8 0x4000000000000000
#define h8 0x8000000000000000

extern UInt64 ReverseMap[256];
extern UInt64 Rank[8];
extern UInt64 File[8];

#define Reverse(X) ((ReverseMap[X & 0xFF] << 56) | \
                   (ReverseMap[(X >> 8)& 0xFF] << 48) | \
                   (ReverseMap[(X >> 16)& 0xFF] << 40) | \
                   (ReverseMap[(X >> 24) & 0xFF] << 32) | \
                   (ReverseMap[(X >> 32) & 0xFF] << 24) | \
                   (ReverseMap[(X >> 40) & 0xFF] << 16) | \
                   (ReverseMap[(X >> 48) & 0xFF] << 8) | \
                   (ReverseMap[(X >> 56) & 0xFF])) \

#define Flip(board) ((board << 56) | \
                    ((board << 40) & 0x00ff000000000000) | \
                    ((board << 24) & 0x0000ff0000000000) | \
                    ((board << 8)  & 0x000000ff00000000) | \
                    ((board >> 8)  & 0x00000000ff000000) | \
                    ((board >> 24) & 0x0000000000ff0000) | \
                    ((board >> 40) & 0x000000000000ff00) | \
                    (board >> 56)) \

#define str(X) to_string(X)
#define Intersect(A, B) ((A) & (~(B)))
#define Union(pieces) (pieces->Pawns   | \
                       pieces->Knights | \
                       pieces->Bishops | \
                       pieces->Rooks   | \
                       pieces->Queen   | \
                       pieces->King    | \
                       pieces->Reserved| \
                       pieces->Reserved2)\


#define PURPLE  "\e[0;34m"
#define PINK    "\e[0;35m"
#define BLUE    "\e[0;36m"
#define GREEN   "\e[0;32m"
#define RED     "\e[1;31m"
#define YELLOW  "\e[1;33m"
#define WHITE   "\e[0m"

UInt64 FlipBoard(UInt64 board);
void DebugPrint(string);
void DebugBoard(UInt64);

#endif // FOUNDATION_HPP
