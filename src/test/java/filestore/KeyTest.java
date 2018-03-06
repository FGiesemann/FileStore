/* FileStore
 * 
 * Copyright (c) 2018, Florian Giesemann
 * All rights reserved.
 * 
 * This code is licensed under the BSD 2-Clause License, which
 * should come with this code in a file LICENSE.
 */

package filestore;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Test;

public class KeyTest {
  private static final String str1 = "9a7f45f42196e03c5fcda43af799bdb7f1d459fc-1";
  private static final String str2 = "38762cf7f55934b34d179ae6a4c80cadccbb7f0a-2";
  
  @Test
  public void testKeyStringConversion() throws InvalidKey {
    assertEquals("Conversion 1", str1, Key.fromString(str1).toString());
    assertEquals("Conversion 2", str2, Key.fromString(str2).toString());
  }
  
  @Test
  public void testInvalidKey() {
    try {
      Key.fromString("38762cf7f55934b34d179ae6a4c80cadccbb7f0a");
      fail("Key from String without sequence number");
      Key.fromString("38762cf7f55934b34d179ae6a4c80cadccbb7f-1");
      fail("Key from too short String");
    } catch (InvalidKey e) {
      // ignored!
    }
  }

}
