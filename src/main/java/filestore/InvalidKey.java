/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

public class InvalidKey extends FileStoreException {

  private static final long serialVersionUID = 7267494552222932579L;

  public InvalidKey(String message) {
    super(message);
  }

  public InvalidKey(String message, Throwable cause) {
    super(message, cause);
  }

  public InvalidKey(Throwable cause) {
    super(cause);
  }

}
