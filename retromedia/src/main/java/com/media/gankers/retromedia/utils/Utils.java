package com.media.gankers.retromedia.utils;

import java.io.BufferedOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by gordon on 2/3/18.
 */

public class Utils {
    public static void flushBytesToFile(byte[] bytes, String destFile) {
        try {
            BufferedOutputStream bos = new BufferedOutputStream(
                    new FileOutputStream(destFile)
            );
            bos.write(bytes, 0, bytes.length);
            bos.flush();
            bos.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
