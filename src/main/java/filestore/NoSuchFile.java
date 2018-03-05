/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

public class NoSuchFile extends FileStoreException {

  private static final long serialVersionUID = -4745053236209498931L;
  
  private Key key;

  public NoSuchFile(Key key) {
    super("No file for given key");
    this.key = key;
  }
  
  public Key getKey() {
    return key;
  }

}
