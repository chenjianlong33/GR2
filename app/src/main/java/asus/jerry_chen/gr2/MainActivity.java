package asus.jerry_chen.gr2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import asus.jerry_chen.gr2.util.ExtractAsset;

public class MainActivity extends AppCompatActivity {

    private TextView mTextView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ExtractAsset mExtractAsset = new ExtractAsset(this);

        mTextView = (TextView) this.findViewById(R.id.text);
        GR_Result GRResult = new GR_Result();
        mTextView.setText("result"+GRResult.getResultFromJni(getFilesDir().getPath()));
    }
}
