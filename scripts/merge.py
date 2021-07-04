# lower app size: 256
# lower app download偏移地址：0x49000 download分区大小：220Kb

# 2:
# app: offset 0x9000 size: 260
# download offset 0x4A000 size: 216

def main():
    length = 1024
    bl_size = 36 * 1024
    version = '0.2.17t'
    module = 'lower'
    with open(f'../Upper/rtboot_f1_{module}.bin', 'rb') as rtboot, open(f'../Upper/rtthread.{module}.{version}.bin', 'rb') as rtt, open(f'../Upper/x.{module}.{version}.bin', 'wb+') as x:
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
