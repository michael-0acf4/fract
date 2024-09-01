# fract

A (very) basic (slightly efficient) arithmetic encoder.

```
Decoded: Hello World
Statistics:
 Original: 11 bytes
 Block size: 4 bytes
 Stored as: 0.203704 (4 bytes)
 Encoded: 39 bytes
 Eff: +254.545 %
 Correct: OK

Decoded: 000000000000000000000000000000000000000000000
Statistics:
 Original: 45 bytes
 Block size: 4 bytes
 Stored as: 0.632005 (4 bytes)
 Encoded: 25 bytes
 Eff: -44.4444 %
 Correct: OK

Decoded: 1234567891234567890
Statistics:
 Original: 19 bytes
 Block size: 4 bytes
 Stored as: 0.11472 (4 bytes)
 Encoded: 63 bytes
 Eff: +231.579 %
 Correct: OK

Decoded: 00000000000000000000011111111111111111111111
Statistics:
 Original: 44 bytes
 Block size: 4 bytes
 Stored as: 0.0105668 (4 bytes)
 Encoded: 40 bytes
 Eff: -9.09091 %
 Correct: OK

Decoded: 001111110000000000111111111111111111111111111111111111111111111111111111111111111
Statistics:
 Original: 81 bytes
 Block size: 4 bytes
 Stored as: 0.118038 (4 bytes)
 Encoded: 49 bytes
 Eff: -39.5062 %
 Correct: OK
```
