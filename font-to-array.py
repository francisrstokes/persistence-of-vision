with open("font.txt", "r") as f:
    font_data = f.read().splitlines()

COLS_PER_CHAR = 5

def char_to_bytes(offset):
    batch = font_data[offset:offset+8]
    out = []

    for x in range(COLS_PER_CHAR):
        value = 0
        for y in range(8):
            if x >= len(batch[y]):
                bit = 0
            else:
                bit = 1 if batch[y][x] == '#' else 0
            value |= bit << (7-y)
        out.append(value)
    return out

def consume_empty_lines(line_offset):
    offset = line_offset
    while offset < len(font_data) and font_data[offset] == "":
        offset += 1
    return offset

offset = 0
letter_index = 0
ord_a = ord('A')

while offset < len(font_data):
    offset = consume_empty_lines(offset)
    if offset >= len(font_data):
        break
    data_str = ", ".join([f"0x{x:02x}" for x in char_to_bytes(offset)])
    print(f"{{ {data_str} }}, // {chr(ord_a + letter_index)} [{letter_index}]")
    offset += 8
    letter_index += 1
