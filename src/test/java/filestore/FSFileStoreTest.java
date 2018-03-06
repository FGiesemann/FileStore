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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class FSFileStoreTest {

  private Path tempDir, inputDir;
  private FileStore filestore;

  @Before
  public void setUp() throws IOException, FileStoreException {
    tempDir = Files.createTempDirectory("FileStore");
    inputDir = Paths.get("src", "test", "resources");
    filestore = new FSFileStore(tempDir);
  }

  @Test
  public void testFileInOut() throws IOException, NoSuchAlgorithmException, DuplicateFile, NoSuchFile {
    Path p = inputDir.resolve("TestFile1.txt");
    Key k = filestore.addFile(p);
    
    InputStream str = filestore.getFile(k);
    InputStream inStr = Files.newInputStream(p);
    assertTrue("File from FileStore is not equal to original", inStreamsEqual(inStr, str));
    inStr.close();
    str.close();
  }
  
  @Test
  public void testDuplicate() throws IOException, DuplicateFile, NoSuchAlgorithmException {
    Path p1 = inputDir.resolve("TestFile1.txt");
    Path p2 = inputDir.resolve("TestFile2.txt");
    Key k1 = filestore.addFile(p1);
    boolean ok = false;
    try {
      filestore.addFile(p2);
    } catch (DuplicateFile d) {
      if (d.getKey().equals(k1))
        ok = true;
    }
    assertTrue("Duplicate file not detected!", ok);
    Path p3 = inputDir.resolve("TestFile3.txt");
    ok = true;
    try {
      filestore.addFile(p3);
    } catch (DuplicateFile d) {
      ok = false;
    }
    assertTrue("Duplicate: false positive", ok);
  }
  
  @Test
  public void testSHACollision() throws IOException, NoSuchAlgorithmException, DuplicateFile {
    Path p1 = inputDir.resolve("shattered-1.pdf");
    Path p2 = inputDir.resolve("shattered-2.pdf");
    Key k1 = filestore.addFile(p1);
    try {
      Key k2 = filestore.addFile(p2);
      assertFalse("Equal keys for different files", k1.equals(k2));
    } catch (DuplicateFile d) {
      fail("SHA collision of non-equal files");
    }
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

  @After
  public void tearDown() throws IOException, InterruptedException {
    if (Files.exists(tempDir))
      Files.walk(tempDir).map(Path::toFile).sorted((o1, o2) -> -o1.compareTo(o2)).forEach(File::delete);
  }

}
