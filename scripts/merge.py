import click
K_BYTE = 1024
def merge(module, input_file, output_file, blsize = 36):
    with open(f'./boot/{module}.bin', 'rb') as rtboot, open(input_file, 'rb') as raw_bin, open(output_file, 'wb+') as merged_bin:
        while True:
            buf = rtboot.read(K_BYTE)
            if not buf:
                break
            merged_bin.write(buf)

        padding_zero_size = blsize * K_BYTE - rtboot.tell()
        merged_bin.write(('\0' * padding_zero_size).encode())

        while True:
            buf = raw_bin.read(K_BYTE)
            if not buf:
                break
            merged_bin.write(buf)

@click.command()
@click.option("--module", type=click.Choice(['upper', 'lower']), help="Module of system")
@click.option("--blsize", type=int, default=36, help="Size of bootloader(KB)")
@click.option("-i", "--input_file", required=True)
@click.option("-o", "--output_file", required=True)
def main(module, blsize, input_file, output_file):
    merge(module, input_file, output_file, blsize)

if __name__ == '__main__':
    main()

# ./rt_ota_packaging_tool_cli-x64 -f ../rtthread.bin -v 0.2.18a -p app -c gzip