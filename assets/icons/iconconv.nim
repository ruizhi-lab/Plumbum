# Plumbum Icon Converter
# Author: DuckSoft <realducksoft@gmail.com>
# Licensed under The Star And Thank Author License (SATA License) & GPL 3.0.

import os
import strformat
import asyncdispatch
import strutils
import sequtils

const linuxIconSizes = [16, 22, 32, 48, 64, 128, 256, 512, 1024]
const windowsIconSizes = [32, 64, 128, 256]
const windowsPackForVistaThreshold = 128
const windowsIconBitDepth = 32
const macOSIconSizes = [16, 32, 128, 256, 512, 1024]

proc svg2PngWithSize(sz: int): Future[int] {.async.} =
    let targetFilename = fmt"plumbum.{sz}.png"
    if os.existsFile(targetFilename):
        echo fmt" => Skipped: {targetFilename}..."
        return 0

    let cmdline = fmt"convert -background none -geometry {sz}x{sz} -density {4*sz} plumbum.svg {targetFilename}"
    echo fmt" => Converting: {targetFilename}..."
    return os.execShellCmd(cmdline)

proc generateLinuxIcon(): Future[void] {.async.} =
    echo "Generating Linux Icons..."
    let sizeFutures = map(linuxIconSizes, svg2PngWithSize)
    discard waitFor all(sizeFutures)

proc generateWindowsIcon(): Future[void] {.async.} =
    echo "Generating Windows Icon..."
    var cmdline = "icotool --create "
    
    var idx = 1
    for size in windowsIconSizes:
        cmdline &= fmt"--index={idx} --width={size} --height={size} --bit-depth={windowsIconBitDepth} --palette-size=0 "
        if size > windowsPackForVistaThreshold:
            cmdline &= "--raw="
        cmdline &= fmt"plumbum.{size}.png "
        idx.inc()
    
    cmdline &= "--output=plumbum.ico"

    let result = os.execShellCmd(cmdline)
    if result == 0:
        echo " => Success!"
    else:
        echo " => Failed!"

proc generateMacOSIcon(): Future[void] {.async.} =
    var cmdline = "png2icns plumbum.icns "
    let macOSIconFilenames = map(macOSIconSizes, proc (size: int): string = fmt"plumbum.{size}.png")
    cmdline &= join(macOSIconFilenames, " ")
    discard os.execShellCmd(cmdline)

proc main() {.async.} =
    echo "Plumbum Icon Converter"
    waitFor generateLinuxIcon()
    waitFor generateWindowsIcon()
    waitFor generateMacOSIcon()

waitFor main()
