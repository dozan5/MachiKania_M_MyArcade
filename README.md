## ソフトの紹介  
　レトロゲームの復刻版　[MyArcade](https://www.myarcadegaming.com/)に  ケンケン様作成の [MachiKania type M](http://www.ze.em-net.ne.jp/~kenken/machikania/typem.html)を組込み  
 　ゲーム機に仕上げました。  
　[小型カラー液晶搭載マイコンゲームシステム](http://www.ze.em-net.ne.jp/~kenken/lcdgame/index.html)を参考にブートローダーとゲームを  
　PIC32MX370F512Hへ移植しました。  

　LCDとの接続は[こちら](https://github.com/dozan5/MachiKania_M_Xevious_lcd)を参照ください。  
 
370版LCDブートローダー対応コントローラー  
　ILI9325(8bit)  
　S6D0154(8bit)  
　ILI9341(8bit)  
　SSD1289(8bit)  
　HX8347(8bit)  
　ST7789(8bit）※　縦、横モード対応   

　ILI9341(16bit)  
　SSD1289(16bit)  
　ST7789(16bit）※　縦、横モード対応  

　　　PICライタ(PICkit3,MPLAB SNAPなど）でbootloader.hexを書き込んでください。  

[LCDゲーム](http://www.ze.em-net.ne.jp/~kenken/lcdgame/index.html)  
　箱入り娘パズル  
　インベーダー  
　パックマン  
　テトリス  
　ペグソリテア  
　写真立て  
　Veludda  
　倉庫番　[原作 BRAIN様](http://braincell.synapse-blog.jp/cell/)  

　おまけ  
　　Xeviousモドキ  
 
 　　　SDカードへhexファイルをコピーしてください。  

## 実機写真   
![](My1.jpg)  
![](My2.jpg)  

MY ARCADE搭載LCDの違い
　左：ROLLING THUNDER搭載 LCD　16bit  
　右：BUBBLE BOBBLE搭載 LCD　8bit  
![](My3.jpg)  
![](My4.jpg)  
![](My5.jpg)  

## 動作環境  
　[MyAcade](https://www.myarcadegaming.com/)の筐体に組込まなくても　[MachiKania type M](http://www.ze.em-net.ne.jp/~kenken/machikania/typem.html)単体でも起動可能です。  
 
　検証済みLCD  
　　　◆2.4インチ aitedo [M024C9325SLD](https://www.aitendo.com/product/15381 )(ILI9325)  
　　　◆2.4インチ aitendo [UL024C0154D8](https://www.aitendo.com/product/16104)(S6D0154)  
　　　◆2.8インチ aitendo [M028C8347D8](https://www.aitendo.com/product/10942)(HX8347-D)  
　　　　　※（Arduino互換のpin配列に変換が必要）  
　　　◆3.2インチ aitendo [M032C9341B3](https://www.aitendo.com/product/11138)(ILI9341)  
　　　　　※（Arduino互換のpin配列に変換が必要）  
　　　◆3.2インチ aitendo [LCD032-2P](https://www.aitendo.com/product/13748)(SSD1289)  
　　　　　※（Arduino互換のpin配列に変換が必要）  
　　　◆MY ARCADE搭載LCD対応(ST7789？)  
　　　　　詳細は[こちら](https://github.com/dozan5/MachiKania_M_Xevious_lcd)を参照ください。  
　　　　　ソフトは縦モードと横モードの両方を用意しました。  

## 起動方法
　　1. 各コントローラーのSDフォルダーのHEXファイル（実行ファイル）をSDカードに  
　　　　保存。  
　　3. MachiKania type MにてLCDブートローダを起動しリストからHEXファイルを  
　　　　選択しLOADする。  
　　3. 一度起動すると次回からは電源を入れると直ぐにゲームが始まります。 
