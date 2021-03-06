"""
*  ____           _
* |  _ \ ___   __| |_ __ _   _ _ __ ___
* | |_) / _ \ / _` | '__| | | | '_ ` _ \
* |  __/ (_) | (_| | |  | |_| | | | | | |
* |_|   \___/ \__,_|_|   \__,_|_| |_| |_|
*
* Licensed under the Mozilla Public License, Version 2.
* Permissions of this weak copyleft license are conditioned on making
* available source code of licensed files and modifications of those files
* under the same license (or in certain cases, one of the GNU licenses).
* Copyright and license notices must be preserved. Contributors
* provide an express grant of patent rights. However, a larger work
* using the licensed work may be distributed under different terms and without
* source code for files added in the larger work.
"""

import binascii
from podrum.utils.BinaryStream import BinaryStream
import os
from podrum.utils.Utils import Utils
import random
import time

class UUID:
    parts = [0, 0, 0, 0]
    version = None

    def __init__(self, part1 = 0, part2 = 0, part3 = 0, part4 = 0, version = None):
        self.parts = [part1, part2, part3, part4]
        self.version = version if version else ((self.parts[1] & 0xf000) >> 12)

    def fromBinary(self, uuid, version = None):
        if len(uuid) != 16:
            raise Exception("Must have exactly 16 bytes")
        stream = BinaryStream(uuid)
        return UUID(stream.getInt(), stream.getInt(), stream.getInt(), stream.getInt(), version)

    def toBinary(self):
        stream = BinaryStream()
        stream.putInt(self.parts[0])
        stream.putInt(self.parts[1])
        stream.putInt(self.parts[2])
        stream.putInt(self.parts[3])
        return stream.buffer

    def fromString(self, uuid, version = None):
        return self.fromBinary(binascii.unhexlify(uuid.strip().replace("/-/g", "").encode()), version)

    def toString(self):
        stream = BinaryStream(binascii.hexlify(self.toBinary()))
        return f"{stream.get(8).decode()}-{stream.get(4).decode()}-{stream.get(4).decode()}-{stream.get(4).decode()}-{stream.get(16).decode()}"

    def getPart(self, partNumber: int):
        if partNumber < 0 or partNumber > 3:
            raise Exception("Invalid UUID part index" + str(partNumber))
        return self.parts[partNumber]
