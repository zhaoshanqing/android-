package com.example.layout;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.GridLayout;
import android.widget.ImageView;


public class MainActivity extends AppCompatActivity {
  /*
   private ImageView [] img=new ImageView[12];
   private int []  imgPath=new int[]{
      R.mipmap.img2,R.mipmap.img3,R.mipmap.img4,R.mipmap.img5,
           R.mipmap.img6,R.mipmap.img7,R.mipmap.img8,R.mipmap.img9,
          R.mipmap.img10,R.mipmap.img11,R.mipmap.bg,R.mipmap.img8
   };


*/


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
     /*   GridLayout layout=(GridLayout)findViewById(R.id.layout);
         for(int i=0;i<imgPath.length;i++){
             img[i]=new ImageView((MainActivity.this));
             img[i].setImageResource(imgPath[i]);
             img[i].setPadding(2,2,2,2);
             ViewGroup.LayoutParams params=new ViewGroup.LayoutParams(116,68);
             img[i].setLayoutParams(params);
             layout.addView(img[i]);


         }
         */
    }
}
