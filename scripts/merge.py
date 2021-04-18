# lower app size: 256
# lower app download偏移地址：0x49000 download分区大小：220Kb

# 2:
# app: offset 0x9000 size: 260
# download offset 0x4A000 size: 216

def main():
    length = 1024
    bl_size = 36 * 1024
    with open('../Upper/rtboot_f1_upper.bin', 'rb') as rtboot, open('../Upper/rtthread.bin', 'rb') as rtt, open('../Upper/x.bin', 'wb+') as x:
        while True:
            buf = rtboot.read(length)
            if not buf:
                break
            x.write(buf)

        btremain = bl_size - rtboot.tell()
        x.write(('\0' * btremain).encode())

        while True:
            buf = rtt.read(length)
            if not buf:
                break
            x.write(buf)


if __name__ == '__main__':
    main()
