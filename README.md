# Lexical-Anaylsis-for-SIC

## 開發平台
Windows 10

## 開發環境
Dev-C++


## 題目說明
1. 讀入一內含SIC程式碼之txt檔檔名後，可將SIC程式碼內容按規則切出 Token，同時將其所在之Table號碼及所在位置寫入輸出檔相應位置內。
2. 可辨識是指令大小寫還是字串大小寫。
3. 註解不翻譯，但有寫入檔案。
4. 註解的符號，(.)句號，有翻譯出來，註解後的內容不翻出。

## 輸入範例
![image](https://user-images.githubusercontent.com/95240041/194577718-6a2a28c2-4b8f-49e8-9032-b407efb385b7.png)


## 流程說明
![image](https://user-images.githubusercontent.com/95240041/194575541-947b575e-54f0-4bf1-86ff-471ff9ee7c2f.png)

Class Readfile:
讀入一內含SIC程式碼之txt檔檔名後【readfile()】，用將SIC程式碼內容按規則切出Token【lexical_analyzer()】，找出各個Token是否有在Table1到Table4內【find_tablenumber()】，若有，則將其所在之Table號碼及所在位置寫入輸出檔相應位置內，若無【put_in_newtable()】，則判斷其屬於Table5到Table7的哪個Table內【which_table()】，將其Token的ASCII code相加後除以 100取餘數作為其位置(hash function，範圍為0~99) 【find_hash()】，若遇到碰撞，則將其放入原位置+1處，再遇到碰撞則依此類推，最後將其所在之Table號碼及所在位置寫入輸出檔相應位置內。

![image](https://user-images.githubusercontent.com/95240041/194575631-17dd9606-c36c-4a34-b7b6-f6cf4d919ea3.png)
Class Table:
用【createtable ()】將Table1.table至Table4.table各自建立在table1至table4內並創建大小為100的table5至table7以便後續程式使用。
【tablenumber()】判斷所讀如檔案為Table1至Table7何者後將其數字回傳。


![image](https://user-images.githubusercontent.com/95240041/194575770-874f34b4-63c4-47ad-96d7-6311944ef834.png)
Class White_Space_Table:
用【insert()】將white space新增至whitespacelist內以便後續程式使用。
用【contain()】判斷此字元是否在whitespacelist內。


Class Delimiter_Table:
用【insert()】將delimiter新增至delimiterlist內以便後續程式使用。
用【contain()】判斷此字元是否在delimiterlist內。



程式設計-使用的DATA STRUCTURE:
![image](https://user-images.githubusercontent.com/95240041/194575880-9455ed32-a332-4df8-a06e-5171890ee2d8.png)
1.	切出來的Token以”行”為單位放在宣告為Tokens類別的Token內
2.	讀出的Table以”檔案”為單位放在宣告為sTable類別的table1~7內
3.	找到Token的Table號碼及所在位置後以”Token”為單位放在宣告為vestor <struct tablenum>類別answer內

## 輸出範例
![image](https://user-images.githubusercontent.com/95240041/194577538-256ea3e5-1f4d-4c6d-9bf1-9441fa65aafc.png)
