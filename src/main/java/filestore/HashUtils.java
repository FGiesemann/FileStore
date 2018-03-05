/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

class HashUtils {

  public static byte[] stringToHash(String str) {
    byte[] hash = new byte[str.length() / 2];
    for (int i = 0; i < str.length() - 1; i += 2)
      hash[i / 2] = (byte) Integer.parseInt(str.substring(i, i + 2), 16);
    return hash;
  }

  private final static char[] hexArray = "0123456789abcdef".toCharArray();

  public static String hashToString(byte[] hash) {
    char[] hexChars = new char[hash.length * 2];
    for (int j = 0; j < hash.length; j++) {
      int v = hash[j] & 0xFF;
      hexChars[j * 2] = hexArray[v >>> 4];
      hexChars[j * 2 + 1] = hexArray[v & 0x0F];
    }
    return new String(hexChars);
  }

  public static byte[] hashFile(Path file) throws NoSuchAlgorithmException, IOException {
    byte[] hash;
    try (InputStream is = Files.newInputStream(file, StandardOpenOption.READ)) {
      hash = hashFile(is);
    }
    return hash;
  }

  public static byte[] hashFile(InputStream is) throws NoSuchAlgorithmException, IOException {
    MessageDigest sha1 = MessageDigest.getInstance("sha-1");
    ByteBuffer buf = ByteBuffer.allocate(4096);
    try (ReadableByteChannel c = Channels.newChannel(is)) {
      while (true) {
        buf.clear();
        int r = c.read(buf);
        if (r == -1)
          break;
        buf.flip();
        sha1.update(buf);
      }
    }
    return sha1.digest();
  }

}
