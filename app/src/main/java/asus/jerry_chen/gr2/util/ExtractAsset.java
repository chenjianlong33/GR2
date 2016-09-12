package asus.jerry_chen.gr2.util;

import android.content.Context;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by Jerry_Chen on 2016/9/12.
 */
public class ExtractAsset {

    private Context mAndroidContext;

    public ExtractAsset(Context context) {
        mAndroidContext = context;


        try {
            for (String fileName : mAndroidContext.getAssets().list("parameter")) {
                extractOpenNIAsset(fileName);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private void extractOpenNIAsset(String filename) throws IOException {
        InputStream is = mAndroidContext.getAssets().open("parameter" + "/" + filename);

        mAndroidContext.deleteFile(filename);
        OutputStream os = mAndroidContext.openFileOutput(filename, Context.MODE_PRIVATE);

        byte[] buffer = new byte[is.available()];
        is.read(buffer);
        is.close();
        os.write(buffer);
        os.close();
    }

}