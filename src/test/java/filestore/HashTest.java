/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.nio.file.Paths;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

import org.junit.Test;

public class HashTest {

  private static final String refStr1 = "38762cf7f55934b34d179ae6a4c80cadccbb7f0a"; // shattered-1.pdf
  private static final String refStr2 = "9a7f45f42196e03c5fcda43af799bdb7f1d459fc"; // TestFile1.txt
  private static final String refStr3 = "9fbc3fafddca353898269a2f4069e4653083bcdb"; // TestFile3.txt

  private static final byte[] refHash1 = new byte[] { 0x38, 0x76, 0x2c, (byte) 0xf7, (byte) 0xf5, 0x59, 0x34, (byte) 0xb3, 0x4d, 0x17, (byte) 0x9a, (byte) 0xe6, (byte) 0xa4, (byte) 0xc8, 0x0c, (byte) 0xad, (byte) 0xcc, (byte) 0xbb, 0x7f, 0x0a };
  private static final byte[] refHash2 = new byte[] { (byte) 0x9a, 0x7f, 0x45, (byte) 0xf4, 0x21, (byte) 0x96, (byte) 0xe0, 0x3c, 0x5f, (byte) 0xcd, (byte) 0xa4, 0x3a, (byte) 0xf7, (byte) 0x99, (byte) 0xbd, (byte) 0xb7, (byte) 0xf1, (byte) 0xd4,
      0x59, (byte) 0xfc };
  private static final byte[] refHash3 = new byte[] { (byte) 0x9f, (byte) 0xbc, 0x3f, (byte) 0xaf, (byte) 0xdd, (byte) 0xca, 0x35, 0x38, (byte) 0x98, 0x26, (byte) 0x9a, 0x2f, 0x40, 0x69, (byte) 0xe4, 0x65, 0x30, (byte) 0x83, (byte) 0xbc,
      (byte) 0xdb };

  @Test
  public void testHashToString() {
    assertTrue("refHash1 to string", HashUtils.hashToString(refHash1).equals(refStr1));
    assertTrue("refHash2 to string", HashUtils.hashToString(refHash2).equals(refStr2));
    assertTrue("refHash3 to string", HashUtils.hashToString(refHash3).equals(refStr3));
  }

  @Test
  public void testStringToHash() {
    assertTrue("refStr1 to hash", Arrays.equals(refHash1, HashUtils.stringToHash(refStr1)));
    assertTrue("refStr2 to hash", Arrays.equals(refHash2, HashUtils.stringToHash(refStr2)));
    assertTrue("refStr3 to hash", Arrays.equals(refHash3, HashUtils.stringToHash(refStr3)));
  }

  @Test
  public void testFileHash() throws NoSuchAlgorithmException, IOException {
    assertTrue("shattered-1.pdf", Arrays.equals(refHash1, HashUtils.hashFile(Paths.get("src", "test", "resources", "shattered-1.pdf"))));
    assertTrue("TestFile1.txt", Arrays.equals(refHash2, HashUtils.hashFile(Paths.get("src", "test", "resources", "TestFile1.txt"))));
    assertTrue("TestFile3.txt", Arrays.equals(refHash3, HashUtils.hashFile(Paths.get("src", "test", "resources", "TestFile3.txt"))));
  }

}
