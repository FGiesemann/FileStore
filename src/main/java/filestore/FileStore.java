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
import java.nio.file.Path;
import java.security.NoSuchAlgorithmException;

/**
 * A FileStore stores files and makes them accessible via keys.
 * 
 * When a file is added to the FileStore, a key is generated for it that allows
 * the user to retrieve the file from the FileStore again. The FileStore will
 * not store duplicates of files. Files are considered duplicates, when their
 * content is identical (same bytes).
 * 
 * @author Florian Giesemann
 */
public interface FileStore {

  /**
   * Add a file into the FileStore.
   * 
   * The FileStore checks, if the same file (i.e., identical file content, same
   * bytes) is already stored in the file store. If not, the new file is added
   * and a key is returned that allows the file to be retrieved from the
   * FileStore again.
   * 
   * @param file
   *          Path to the file to add.
   * @return Key for the file in the FileStore.
   * @throws IOException
   *           In case the file could not be read or the FileStore was not able
   *           to store it internally.
   * @throws DuplicateFile
   *           If the file to be added is already stored in the FileStore.
   * @throws NoSuchAlgorithmException
   *           The FileStore relies on the SHA-1 message digest algorithm. If it
   *           cannot be instantiated, this exception is thrown.
   */
  public Key addFile(Path file) throws IOException, DuplicateFile, NoSuchAlgorithmException;

  /**
   * Gives access to a file stored in the FileStore.
   * 
   * The file can be accessed through a InputStream object generated for it.
   * 
   * @param key
   *          Key of the file to retrieve.
   * @return An InputStream from which the file data can be read.
   * @throws IOException
   *           If the file could not be accesses.
   * @throws NoSuchFile
   *           If no file with the given Key could be found in the FileStore.
   */
  public InputStream getFile(Key key) throws IOException, NoSuchFile;

  /**
   * Remove a file from the FileStore.
   * 
   * @param key
   *          Key of the file to remove.
   * @throws IOException
   *           If the file could not be removed.
   * @throws NoSuchFile
   *           If no file with the given Key could be found in the FileStore.
   */
  public void removeFile(Key key) throws IOException, NoSuchFile;

}