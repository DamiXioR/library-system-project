# Utilities

# UUID v4 Generator in C++

This project implements a **UUID v4 generator** in pure C++, fully compliant with the [RFC4122](https://www.rfc-editor.org/rfc/rfc4122.html) standard.

A UUID (Universally Unique Identifier) is a **128-bit unique identifier** used for databases, distributed systems, or object identification in software.

---

## UUID v4 Format

A UUID v4 looks like this: `xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx`

- 128 bits total (4 × 32-bit words)  
- **Version** = 4 (third group, bits 12–15)  
- **Variant** = RFC4122 (fourth group, bits 31–30)  
- Remaining bits = **random**  

**Example:**
550e8400-e29b-41d4-8716-446655440000

The UUID is divided into 5 groups:

| Group | Hex          | Bits    | Description                                                                       |
| ----- | ------------ | ------- | --------------------------------------------------------------------------------- |
| 1     | 550e8400     | 32 bits | `time_low` – lower part of timestamp / random in v4                               |
| 2     | e29b         | 16 bits | `time_mid` – middle part of timestamp / random in v4                              |
| 3     | **4**1d4         | 16 bits | `time_hi_and_version` – upper part of timestamp + **version**                     |
| 4     | **8**716         | 16 bits | `clock_seq_hi_and_reserved + clock_seq_low` – **variant** + remaining random bits |
| 5     | 446655440000 | 48 bits | `node` – random in v4, identifies node in v1                                      |


- **`4`** → version v4, located in the **first hex digit of the third group** (`time_hi_and_version` field)  
- **`8`** → variant RFC 4122, located in the **first hex digit of the fourth group** (`clock_seq_hi_and_reserved` field); its **two most significant bits** are `10`
- Other bits = random

> **Note:** In UUID v4, the names of the groups (`time_low`, `time_mid`, `time_hi_and_version`, `clock_seq_hi_and_reserved`, `node`) are **historical** and come from the original UUID structure defined for time-based UUIDs (v1).  
> 
> For random UUIDs, these names do **not affect the values**; the groups are just a convenient way to divide the 128 bits for representation. Only the **version and variant fields** need to follow the specification.

---

## How the Generator Works (Step by Step)

### 1. Generate random data

We create a 128-bit random value using 4 × 32-bit integers.

Example for one word: `data[3] = 0x55440000 = 0101 0101 0100 0100 0000 0000 0000 0000 (binary)`

---

### 2. Set the version (bits 12–15)

Version 4 is set, which means that the UUID is generated from random values.

`data[1] = (data[1] & 0xFFFF0FFF) | 0x00004000`

**Example for clarity:**

| Step | Operation | Binary Representation | Description |
|------|-----------|----------------------|-------------|
| 1 | `data[1]` (before) | `1000 0111 0110 0101 0100 0011 0010 0001` | Original 32-bit segment |
| 2 | AND mask `0xFFFF0FFF` | `1111 1111 1111 1111 0000 1111 1111 1111` | Mask to clear version bits |
| 3 | After AND | `1000 0111 0110 0101 0000 0011 0010 0001` | Version bits cleared |
| 4 | OR mask `0x00004000` | `0000 0000 0000 0000 0100 0000 0000 0000` | Mask to set version to `0100` |
| 5 | After OR | `1000 0111 0110 0101 0100 0011 0010 0001` | Version set to `0100` (UUID v4) |

✅ Result: bits 12–15 = 0100, which is version 4

---

### 3. Set the variant (bits 31–30 of the fourth group)

---

The highest two bits are cleared, and the value 8 is set as the variant, which means that the UUID is compliant with RFC4122.

`data[2] = (data[2] & 0x3FFFFFFF) | 0x80000000`

`& 0x3FFFFFFF` → clear the highest two bits

`| 0x80000000` → set the highest bit to 1, second-highest remains 0

**Example for clarity:**

| Step | Operation | Binary Representation | Description |
|------|-----------|----------------------|-------------|
| 1 | `data[2]` (before) | `1010 1011 1100 1101 1110 1111 0000 0001` | Original 32-bit segment |
| 2 | AND mask `0x3FFFFFFF` | `0011 1111 1111 1111 1111 1111 1111 1111` | Mask to clear the highest two bits (variant bits) |
| 3 | After AND | `0010 1011 1100 1101 1110 1111 0000 0001` | Highest two bits cleared |
| 4 | OR mask `0x80000000` | `1000 0000 0000 0000 0000 0000 0000 0000` | Mask to set the variant to `10xx` (RFC 4122 variant) |
| 5 | After OR | `1010 1011 1100 1101 1110 1111 0000 0001` | Variant bits set; UUID compliant with RFC 4122 |

The variant is determined by the **two most significant bits of the first byte of the fourth group**.  
For RFC 4122 UUIDs, the first hex digit of this byte can only be: **8, 9, a, or b**.

✅ Result: variant bits = 10 → RFC4122

### 4. Format as a string

Split the 128-bit value into standard UUID groups:
`xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx`

### 5. SUMMARY

```text
UUID v4 = 122 random bits + 4 version bits + 2 variant bits
Cross-platform: works on Windows, Linux, macOS
No external libraries needed
UUID v4: 550e8400-e29b-41d4-a716-446655440000
Version = 4
Variant = 10 (RFC4122)
Remaining bits = random
```

## Additional information

### Hex ↔ Binary ↔ Bits ↔ Bytes

| Hex      | Binary                               | Number of bits | Number of bytes |
|----------|--------------------------------------|----------------|----------------|
| F        | 1111                                 | 4              | 0.5            |
| FF       | 1111 1111                            | 8              | 1              |
| FFFF     | 1111 1111 1111 1111                  | 16             | 2              |
| FFFFFF   | 1111 1111 1111 1111 1111 1111        | 24             | 3              |
| FFFFFFFF | 1111 1111 1111 1111 1111 1111 1111 1111 | 32          | 4              |

### UUID versions

| Version | Method                      | Notes                  |
| ------- | --------------------------- | ---------------------- |
| 1       | Time + MAC address          | Allows sorting by time |
| 2       | DCE Security                | Rarely used            |
| 3       | MD5 hash of name+namespace  | Deterministic          |
| 4       | Random                      | Most common, simple    |
| 5       | SHA1 hash of name+namespace | Deterministic          |


### Bitwise operators

#### Bitwise AND (`&`)

Bitwise AND compares each bit of two numbers:

| Original Bit | Mask Bit | Result | Explanation              |
|--------------|----------|--------|--------------------------|
| 0            | 0        | 0      | Mask forces 0            |
| 0            | 1        | 0      | Original bit kept (0)    |
| 1            | 0        | 0      | Mask forces 0            |
| 1            | 1        | 1      | Original bit kept (1)    |

**Rule:**  
- `1` in mask → keep original bit  
- `0` in mask → set bit to 0  

---

#### Bitwise OR (`|`)

Bitwise OR also compares each bit position:

| Original Bit | Mask Bit | Result | Explanation                  |
|--------------|----------|--------|------------------------------|
| 0            | 0        | 0      | Bit unchanged                |
| 0            | 1        | 1      | Mask forces 1                |
| 1            | 0        | 1      | Bit unchanged                |
| 1            | 1        | 1      | Mask forces 1 (already 1)    |

**Rule:**  
- `1` in mask → set bit to 1  
- `0` in mask → leave bit unchanged  
