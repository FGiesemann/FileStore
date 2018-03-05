/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

import java.nio.file.Path;

public class DuplicateFile extends FileStoreException {
  
  private static final long serialVersionUID = -5936793964106234135L;
  
  private Path filePath;
  private Key key;

  DuplicateFile(Path file, Key k) {
    super("Duplicate file");
    filePath = file;
    key = k;
  }
  
  public Path getFilePath() {
    return filePath;
  }
  
  public Key getKey() {
    return key;
  }

}
