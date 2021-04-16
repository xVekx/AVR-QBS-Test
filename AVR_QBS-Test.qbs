import qbs
import qbs.FileInfo

Project {
    //name: "AVR"
    Product {

        name: "atmega328p_test"
        type: ["elf","lss","size","hex","eep"]

        Group {
            name: "sources"
            files: ["*.c"]
            fileTags: ['c']
        }

        Group {
            name: "h"
            files: ["*.h","inc_test/*.h"]
            fileTags: ['h']
        }

        Group {
            name: "AVR"
            property string path : '/usr/lib/avr/include/'
            files: [
                path + "*.h",
                path + "avr/*.h",
                path + "util/*h"
            ]
        }

        property string avr_compiler: "avr-gcc"
        property string avr_objcopy: "avr-objcopy"
        property string avr_objdump: sourceDirectory + "/avr-objdump-shell-script.sh"
        property string avr_size: "avr-size"

        property var avr_compiler_options:  [
              "-mmcu=atmega328p",
              "-Wall",
              "-Wstrict-prototypes",
              "-g",
              "-Os",
              "-lm",
              "-std=gnu99",
              "-fpack-struct",
              "-fshort-enums",
              "-ffunction-sections",
              "-fdata-sections",
              "-funsigned-char",
              "-funsigned-bitfields",
              "-fno-exceptions",
            ];

        property var avr_linker_options:  [
              "-mmcu=atmega328p",
              "-mrelax",
              "-Wl,--gc-sections",
              "-mcall-prologues",
              //"-Wl,--strip-all",
              "-Os"
            ];


        Rule {
            inputs: ["c"]
            Artifact {
                fileTags: ['obj']
                filePath:   '.obj/' +
                            qbs.getHash(input.baseDir) +
                            '/' +
                            input.fileName + '.o'
            }
            prepare: {
                var args = [];
                args = args.concat(product.avr_compiler_options);
                args.push('-I' + product.sourceDirectory + '/inc_test');
                args.push('-c');
                args.push(input.filePath);
                args.push('-o');
                args.push(output.filePath);
                //console.info("Hello!");
                print("Hello!");
                var cmd = new Command(product.avr_compiler, args);
                cmd.description =   '========\ncompiling ' + input.fileName +
                                '\n' + product.avr_compiler + ' ' + args;
                return cmd;
            }
        }

        Rule {
            multiplex: true
            inputs: ['obj']
            Artifact {
                fileTags: ['elf']
                filePath: product.name + '.elf'
            }
            prepare: {
                var args = [];
                args = args.concat(product.avr_linker_options)
                for (i in inputs["obj"])
                            args.push(inputs["obj"][i].filePath);
                args.push('-o');
                args.push(output.filePath);
                var cmd = new Command(product.avr_compiler, args);
                cmd.description = '========\nlinking ' + product.name +
                                  '\n' + product.avr_compiler + ' ' + args;
                return cmd;
            }
        }

        Rule {
            inputs: "elf"
            Artifact {
                fileTags: ["hex"]
               filePath: "../../" + product.name + ".hex"
            }
            prepare: {
                    var args = [
                    "-R",".eeprom",
                    "-R",".fuse",
                    "-R",".lock",
                    "-R",".signature",
                    "-O","ihex",
                    input.filePath,output.filePath];
                    var cmd = new Command(product.avr_objcopy, args);
                    cmd.description = "\n\nFLASH" + args;
                    return cmd;
                }
            }

        Rule {
            inputs: "elf"
            Artifact {
                fileTags: ["eep"]
               filePath: "../../" + product.name + ".eep"
            }
            prepare: {
                    var args = [
                    "-j",
                    ".eeprom",
                    "--no-change-warnings",
                    "--change-section-lma",
                    ".eeprom=0",
                    "-O",
                    "ihex",
                    input.filePath, output.filePath];
                    var cmd = new Command(product.avr_objcopy, args);
                    cmd.description = "EEPROM";
                    return cmd;
            }
        }

        Rule {
            inputs: "elf"
            Artifact {
                fileTags: ["lss"]
                filePath: "../../" + product.name + ".lss"
            }
            prepare: {
                var args = [input.filePath,output.filePath];
                var cmd = new Command(product.avr_objdump,args);
                cmd.description = "Disassembly listing for " + cmd.workingDirectory;
                cmd.silent = true;
                return cmd;
            }
        }

        Rule {
            inputs: "elf"
            Artifact {
                fileTags: ["size"]
                filePath: ["-"]
            }
            prepare: {
                var args = ["--format=avr","--mcu=atmega328p",input.filePath];
                var cmd = new Command(product.avr_size, args);
                cmd.silent = true;
                return cmd;
            }
        }
    }
}
