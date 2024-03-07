TGA batch converter
===================

### Usage
```tgaBatchConverter ./image.tga <format>```

or

```tgaBatchConverter ./Images <format>```

## Documentation
  
| Formats | Descriptions |
| --- | --- |
| 0 | Uncompressed, 8-bit color-mapped image. |
| 1 | Uncompressed, 24-bit or 32-bit true-color image. |
| 2 | Uncompressed, 15-bit or 16-bit true-color image. |
| 3 | Uncompressed, 16-bit black-and-white image. |
| 4 | Uncompressed, 8-bit black-and-white image. |
| 5 | Run-length encoded, 8-bit color-mapped image. |
| 6 | Run-length encoded, 24-bit or 32-bit true-color image. |
| 7 | Run-length encoded, 15-bit or 16-bit true-color image. |
| 8 | Run-length encoded, 16-bit black-and-white image. |
| 9 | Run-length encoded, 8-bit black-and-white image. |

### Notes
Using "0" and "5" formats will fail if the image has over 256 colors.
