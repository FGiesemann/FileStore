/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

import java.util.List;

/**
 * A Key identifies a file stored in a FileStore.
 * 
 * The key allows the user to retrieve a file from a FileStore.
 * 
 * The key does not identify the FileStore for which it is valid.
 * 
 * @author Florian Giesemann
 */
public class InternalKey extends Key {

  public InternalKey(byte[] hash, int seqNo) {
    super(hash, seqNo);
  }

  public static int getMaxSeqNo(List<Key> keys) {
    int seqNo = 0;
    for (Key k : keys) {
      if (k.seqNo > seqNo)
        seqNo = k.seqNo;
    }
    return seqNo;
  }

  public static InternalKey fromKey(Key k) {
    return (InternalKey) k;
  }

}
