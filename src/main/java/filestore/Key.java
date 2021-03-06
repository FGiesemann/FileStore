/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

import java.util.Arrays;
import java.util.List;

/**
 * Keys are used by the FileStore to identify files.
 * 
 * When a file is added to the FileStore, a key is generated that identifies the
 * file inside the FileStore.
 * 
 * @author Florian Giesemann
 */
public class Key {

  protected byte[] hash;
  protected int seqNo;

  public Key(byte[] hash, int seqNo) {
    this.hash = Arrays.copyOf(hash, hash.length);
    this.seqNo = seqNo;
  }
  
  public Key(byte[] hash, List<Key> sameHashKeys) {
    this.hash = Arrays.copyOf(hash, hash.length);
    this.seqNo = getMaxSeqNo(sameHashKeys) + 1;
  }
  
  @Override
  public int hashCode() {
    final int prime = 31;
    int result = 1;
    result = prime * result + Arrays.hashCode(hash);
    result = prime * result + seqNo;
    return result;
  }

  @Override
  public boolean equals(Object obj) {
    if (this == obj)
      return true;
    if (obj == null)
      return false;
    if (getClass() != obj.getClass())
      return false;
    Key other = (Key) obj;
    if (!Arrays.equals(hash, other.hash))
      return false;
    if (seqNo != other.seqNo)
      return false;
    return true;
  }
  
  private static int getMaxSeqNo(List<Key> keys) {
    int seqNo = 0;
    for (Key k : keys) {
      if (k.seqNo > seqNo)
        seqNo = k.seqNo;
    }
    return seqNo;
  }

  /**
   * Generate a Key from a string representation.
   * 
   * Takes a string representation, which should have been generated via
   * {@link Key#fromString(String)} before, and converts it into a Key object,
   * that can be used with a FileStore.
   * 
   * @param str
   *          String representation of the key.
   * @return Key object from the string representation.
   * @throws InvalidKey
   *           If the string does not represent a valid key.
   */
  public static Key fromString(String str) throws InvalidKey {
    int sep = str.indexOf('-');
    if (sep == -1)
      throw new InvalidKey("The given string does not represent a valid key: " + str);
    String hashStr = str.substring(0, sep);
    if (hashStr.length() != 40)
      throw new InvalidKey("The given string does not represent a valid key: " + str);
    int seqNo;
    byte[] hash;
    try {
      seqNo = Integer.parseInt(str.substring(sep + 1));
      hash = HashUtils.stringToHash(hashStr);
    } catch (NumberFormatException e) {
      throw new InvalidKey("The given string does not represent a valid key", e);
    }
    return new Key(hash, seqNo);
  }

  @Override
  public String toString() {
    return HashUtils.hashToString(hash) + String.format("-%d", seqNo);
  }

}
