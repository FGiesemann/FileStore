/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

public class FileStoreException extends Exception {

  public FileStoreException() {
    super();
  }

  public FileStoreException(String message) {
    super(message);
  }
  
  public FileStoreException(String message, Throwable cause) {
    super(message, cause);
  }
  
  public FileStoreException(Throwable cause) {
    super(cause);
  }

  private static final long serialVersionUID = 5001606955136207453L;

}
