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
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class FSFileStore implements FileStore {

  private static final int FOLDER_NAME_LENGTH = 2;

  private Path basePath;

  public FSFileStore(Path basePath) throws IOException, FileStoreException {
    this.basePath = basePath;
    if (Files.exists(this.basePath)) {
      if (Files.isDirectory(this.basePath)) {
        // everything ok
      } else {
        throw new FileStoreException("File store base path already exists, but is not a directory!");
      }
    } else {
      Files.createDirectories(this.basePath);
    }
  }

  /*
   * (non-Javadoc)
   * 
   * @see filestore.FileStore#addFile(java.nio.file.Path)
   */
  public Key addFile(Path file) throws IOException, DuplicateFile, NoSuchAlgorithmException {
    byte[] hash = HashUtils.hashFile(file);
    List<Key> possibleDups = checkForDuplicate(file, hash);
    Key internalKey = new Key(hash, possibleDups);
    return importFile(file, internalKey);
  }

  private Key importFile(Path file, Key key) throws IOException {
    Path internalPath = getFilePath(key);
    Path fileDir = getFileDirectory(key.toString());
    if (!Files.exists(fileDir))
      Files.createDirectory(fileDir);
    Files.copy(file, internalPath);
    return key;
  }

  private List<Key> checkForDuplicate(Path file, byte[] hash) throws IOException, DuplicateFile {
    List<Key> possibleDups = enumerateFilesByHash(hash);
    for (Key k : possibleDups) {
      try {
      if (filesEqual(file, k))
        throw new DuplicateFile(file, k);
      } catch (NoSuchFile e) {
        /* ignored: This should not happen, if this FileStore instance is the only one that
         * accesses the file system, as it should be!
         */
      }
    }
    return possibleDups;
  }

  /*
   * (non-Javadoc)
   * 
   * @see filestore.FileStore#getFile(filestore.Key)
   */
  public InputStream getFile(Key key) throws IOException, NoSuchFile {
    Path file = getFilePath(key);
    if (Files.exists(file))
      return Files.newInputStream(file);
    else
      throw new NoSuchFile(key);
  }

  /*
   * (non-Javadoc)
   * 
   * @see filestore.FileStore#removeFile(filestore.Key)
   */
  public void removeFile(Key key) throws IOException, NoSuchFile {
    Path file = getFilePath(key);
    if (Files.exists(file))
      Files.delete(file);
    else
      throw new NoSuchFile(key);
  }

  List<Key> enumerateFilesByHash(byte[] hash) throws IOException {
    String hashStr = HashUtils.hashToString(hash);
    Path dir = getFileDirectory(hashStr);
    List<Key> keyList = new ArrayList<>();
    if (!Files.exists(dir))
      return keyList;
    DirectoryStream<Path> dirStream = Files.newDirectoryStream(dir);
    for (Path p : dirStream) {
      String name = p.getFileName().toString();
      try {
        if (name.startsWith(hashStr))
            keyList.add(Key.fromString(name));
      } catch (InvalidKey e) {
        /* Invalid file in the FileStore file system. Should not happen. */
      }
    }
    return keyList;
  }

  boolean filesEqual(Path file, Key k) throws IOException, NoSuchFile {
    Path internalPath = getFilePath(k);
    if (Files.size(internalPath) != Files.size(file))
      return false;
    InputStream intern = getFile(k);
    InputStream outside = Files.newInputStream(file);
    boolean equal = inStreamsEqual(intern, outside);
    intern.close();
    outside.close();
    return equal;
  }

  private boolean inStreamsEqual(InputStream stream1, InputStream stream2) throws IOException {
    byte[] block1 = new byte[4096];
    byte[] block2 = new byte[4096];
    int r1 = stream1.read(block1);
    while (r1 != -1) {
      int r2 = stream2.read(block2);
      if (r1 != r2)
        return false;
      if (!Arrays.equals(block1, block2))
        return false;
      r1 = stream1.read(block1);
    }
    return true;
  }

  Path getFilePath(Key k) {
    String keyStr = k.toString();
    return getFileDirectory(keyStr).resolve(keyStr);
  }

  Path getFileDirectory(String dir) {
    return basePath.resolve(dir.substring(0, FOLDER_NAME_LENGTH));
  }

}
