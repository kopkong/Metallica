#include "DataHelper.h"
#include "engine.inl"

const char delim = ',';

bool DataHelper::initAll()
{
	loadHeroCards();

	return true;
}


bool DataHelper::loadHeroCards()
{
	wifstream fin = loadUTF8File("heroes.txt");
	fin.imbue(gel::stdx::utf8_locale);

	if(fin.is_open()==false)
	{
		SDLDie("Unable to open heroes card!");
	}

	wchar_t wc;
	wstring line;
	while(fin.get(wc))
	{
		// Skip BOM of UTF8
		if(wc == 65279)
			continue;

		line.push_back(wc);

		if(wc == char(10))	// End of line
		{
			// HeroCard hero
			const unsigned int lineItemCount = 10;
			wstring items[lineItemCount];
			splitWString(line,delim,items,lineItemCount);

			// Construct Hero Card
			wstring str1 = items[0];  //name
			wstring str2 = items[1];  // 字
			int n1 = convertWStrToInt(items[2]);	// born year
			int n2 = convertWStrToInt(items[3]);	// 统率
			int n3 = convertWStrToInt(items[4]);	// 武力
			int n4 = convertWStrToInt(items[5]);	// 智力
			int n5 = convertWStrToInt(items[6]);	// 政治
			int n6 = convertWStrToInt(items[7]);	// 魅力
			string img(items[8].begin(),items[8].end());	// Face Image
			string str3(items[9].begin(),items[9].end());	// logName

			HeroCard card;//(str1,str2,n1,n2,n3,n4,n5,n6);
			//card.mName = str1;
			//card.mCourtesyName = str2;
			//card.mBornYear = n1;
			//card.mTongLv = n2;
			//card.mWuLi = n3;
			//card.mZhiLi = n4;
			//card.mZhengZhi = n5;
			//card.mMeili = n6;
			//card.imgID = img+".jpg";
			//card.mLogName = str3;
			

			mHeroCards.push_back(card);

			line.clear();
		}

	}

	return true;
}

