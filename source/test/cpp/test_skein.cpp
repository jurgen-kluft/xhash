#include "xbase/x_target.h"
#include "xbase/x_string_ascii.h"

#include "xhash/x_skein.h"

#include "xunittest/xunittest.h"

using namespace xcore;

namespace SkeinTestVectors
{
	// Original test-vectors
	struct Vector
	{
		u32				Len;
		char const*		Msg;
		char const*		Digest;
	};

	static inline xbyte CharToByte(char ch)
	{
		xbyte ich = 0;
		if ((ch >= '0') && (ch <= '9'))
			ich = ch - '0';
		else if ((ch >= 'A') && (ch <= 'F'))
			ich = ch - 'A' + 10;
		else if ((ch >= 'a') && (ch <= 'f'))
			ich = ch - 'a' + 10;
		return ich;
	}

	static inline xbyte TwoCharsToByte(char h, char l)
	{
		xbyte ich = (CharToByte(h) << 4) | (CharToByte(l));
		return ich;
	}

	static u32 TextMsgToByteMsg(char const* _textmsg, u32 _length, xbyte* _bytemsg)
	{
		for (s32 j = 0; j < _length; ++j)
			_bytemsg[j] = 0;

		u32 i = 0;
		u32 d = 0;
		while (true)
		{
			char const ch = _textmsg[i];
			if (ch == '\0')
				break;

			xbyte const b = CharToByte(ch);
			for (s32 j = 0; j < _length - 1; j++)
				_bytemsg[j] = (_bytemsg[j] << 4) | (_bytemsg[j + 1] >> 4);
			_bytemsg[_length - 1] = (_bytemsg[_length - 1] << 4) | b;
			d++;
			i += 1;
		}
		return (d + 1) / 2;
	}

	static xbyte ByteMsg[4096];

	static Vector Tests[] = {
		  { 2048, "724627916C50338643E6996F07877EAFD96BDF01DA7E991D4155B9BE1295EA7D21C9391F4C4A41C75F77E5D27389253393725F1427F57914B273AB862B9E31DABCE506E558720520D33352D119F699E784F9E548FF91BC35CA147042128709820D69A8287EA3257857615EB0321270E94B84F446942765CE882B191FAEE7E1C87E0F0BD4E0CD8A927703524B559B769CA4ECE1F6DBF313FDCF67C572EC4185C1A88E86EC11B6454B371980020F19633B6B95BD280E4FBCB0161E1A82470320CEC6ECFA25AC73D09F1536F286D3F9DACAFB2CD1D0CE72D64D197F5C7520B3CCB2FD74EB72664BA93853EF41EABF52F015DD591500D018DD162815CC993595B195", "56ED1B402B0F1E382CE309E1E8BA0E23204954BAF2831F15796F3E2B4665BCB42565B910D8F00B6744B2CDB7300E1B8405D7980ED0610182B1858961C83EDC54" }
		, { 2300, "68F891C11459B2E9B71774E2B8A2A5C3C9CC36072E3498498496F1C7901684F3E9DACF13A3F1BAE22140DEE5253732E4D4196B534F675264B53D38659727797F0A18910CCB5B48FE2346C2E998B6537357AF8D15826FCB57BA804FE143E765F4680A0B28A9E3716A59ABC60EF253E357A4784FF1BE4680C82D797813CE50355D8FDB3C75936CC33E1717B99D9A8ADE9D0EA9172662B708EBCBC31C05064FE67B287C56D01C12411E9B890AD16238B36E192B15CD86D26F4EFDE5B523D71656F5CEA6CA73BDC04FEB973D303BBAF4C0264761092E23220CEB8359FC91C1D918FB3F32DBBA92DEAC1C71DA8BC4BE806803F6E7970FA64721C645EED4C2BED7EEFA2B720931FDBD6C67B83756B1CE51C51F839C250AD900B9D49FE5188FC4A2B5D0", "E3E04CA6C02921DF4C0E23AE54D3F7A331067D8CFF58D325A3A9F80FC467458AE1DA59228BBEFC107EE97064DD93A07D1F6B51A352530EC1B8D516FC875FCD90" }
		, { 2111, "919FE5E7F35F64A7487649E564771DBBF10AE204ECC2181312D1A79FB579297C94F0DB9EAAE9E009A4F02057AF2C973C5DAFA7B60154371A5D2C8E992FB6429176F8424B1A866BC1D1BED00438E97FAB42040DCACDEF7CA9FC2033059B8898BB40CCFB2634B051797BDF3B915C503EC81839AD01E0F4F2F871EFF2008D40011730BE7A47888E7955A806876BE120CB0F3A139A3620154ECC6482A70F5629F6A9D3341BE6FBBF48E5AA0C53589A04F057DD44268AFFCABF75ADFC549F73F454264D46A98CCA80E3000C7446853DD5B430C9344E87E3230555B09FB3E7E64B5AD3989293AC0FEEC0E75F909696F028A5525D26DDEA5D2B2C813FB3613DFF38CE23209285CC77C60860", "9EEE738A5C7E3D11F333D9B6901DF9E258942A2602A91B716D470D4799309A9A0EDB29012D2C7ADCD55637963D4CFC8147AE4EC7A041FA64E928B74F68AE6764" }
		, { 2174, "BD687B6D6684949FBD52A8196DF5D5927B403DEE88A47E4E4B52071BDDA6A6284A9EA1EB84950BA17A60277B0FEA83C930E5619D5A6D232901B9D3A696662C7FD5F1349B494BC166C67B717C06D03E9FB05DB9EE292D4792F459E0ACC08369E3D14684CFF388C940F30738FB71D97EFEB51C7DB553CB85B6FC18429815EC71420822151BB8F5135DD698EFE3D3118963EE194F94682DFA62EC9D3BCB5F826DA790779704DF2AC5C8A9FA2B50A42A87ED2FBADF1E1C8E55689531786A9858DE9E88009E56873FD2D431FCFC99B85574342DC7E78406678BDE94B9A06974DFF154EAC3BC7977AC7C123EBCDAC0641A69B792058BA373EDD1BA45E3339C2ABC032993B25F4E3BFA3D5FF620C621D94C40F8", "798448399DEB3A7FAFEE4C932C619171DCBBDAEF417551D59742D94C267BC7ED69FA1E106E36AE2EBC9812B80DE3A226E0FB779A1BF55AAD1CD77BAE7FE43D42" }
		, { 2237, "D13409007A6B3242CBAD4CE01D9AED77EFEF0A7725B96A30097EA092FA1E49A136DE03F3F348464C9BEA033216F380440C7EC81E284738DCC640D485ED32F5D7DD1CF936C1677CBB3CDD8E0E001783FD5A3F388AE6EF7085FF9A22FA722C7FCCF4CDC239B200D5D11884B565D01C84E4D563C6623C241C5AE0812CE8F9201BBADE48198378A3E232EF3E10C03022620A266D0A346A6A22F202333A94D2CEF495DBA102B133B8237449C3350BF80EE51B6EE8A972CCFC3ED5FD7790444DA253922EEF6C611180F91DF1B6E58E843D318D94A958A017590B14D383C0F385F9CF2514E8AE1EA749795E10F991E3FB744C6030EF6B02989E8EFFBF8E8BF31FC39B692C517C7C012ECBD0E0785BDEF4801C4E036C98C6CD0C9328", "1CA9EFFA23C442DD8A4964E703E823204584952CF9753466C3ED674CC37D6912ABF71DD5D1BEED92210D5540CAC63491E78643D776278857A2B67B9FD2A9C5EC" }
		, { 2363, "7D9222C58DC49F14BFD7198A9A1C338D17201C007822A91DCD262860364CA1DA8C0056033EB58E406E36D5A4F6F7E9D98BE57126C9FE7676B58FDF0F9899432FF78DE2AB65C9000071EBA6967123F97BA3DC1F3825D3C8C5148EAD7AEF0334F40CB0F6B982CB7C99CA39E1B4B2E3A3541683D1EC5560466F52175518390D38C7461C116DB2D56EF913784D2E8B20959BBD6F8F3282C597D94A1EDBFA8F25089E9D2E8DC465EAD90FA23E4388BD6BAFE22632749B7AAEA53D5CBCF9678227ADC3F4109F1849AC2539B6F2B25B4D8EDAB41E8BFCFC337728DC48A8EA5119115FC1B133300D68231C96A9D518F6DC3CA51581309C53F49510FE18F608A215069D41F2CFC84E53A9347FD723DFF9D3F5006F7C0B18441A29BDEF7725920260B1613C6532A0A994B488E0", "520FD5F5C64ECEE866F69CB024574D4D7CAFE63CCA2F56D3B2C05BC57FBC955318990F3CDB64268C4C319B4E078D66569862A1CC93738DF148B858692E5BD9A9" }
		, { 4064, "4FBDC596508D24A2A0010E140980B809FB9C6D55EC75125891DD985D37665BD80F9BEB6A50207588ABF3CEEE8C77CD8A5AD48A9E0AA074ED388738362496D2FB2C87543BB3349EA64997CE3E7B424EA92D122F57DBB0855A803058437FE08AFB0C8B5E7179B9044BBF4D81A7163B3139E30888B536B0F957EFF99A7162F4CA5AA756A4A982DFADBF31EF255083C4B5C6C1B99A107D7D3AFFFDB89147C2CC4C9A2643F478E5E2D393AEA37B4C7CB4B5E97DADCF16B6B50AAE0F3B549ECE47746DB6CE6F67DD4406CD4E75595D5103D13F9DFA79372924D328F8DD1FCBEB5A8E2E8BF4C76DE08E3FC46AA021F989C49329C7ACAC5A688556D7BCBCB2A5D4BE69D3284E9C40EC4838EE8592120CE20A0B635ECADAA84FD5690509F54F77E35A417C584648BC9839B974E07BFAB0038E90295D0B13902530A830D1C2BDD53F1F9C9FAED43CA4EED0A8DD761BC7EDBDDA28A287C60CD42AF5F9C758E5C7250231C09A582563689AFC65E2B79A7A2B68200667752E9101746F03184E2399E4ED8835CB8E9AE90E296AF220AE234259FE0BD0BCC60F7A4A5FF3F70C5ED4DE9C8C519A10E962F673C82C5E9351786A8A3BFD570031857BD4C87F4FCA31ED4D50E14F2107DA02CB5058700B74EA241A8B41D78461658F1B2B90BFD84A4C2C9D6543861AB3C56451757DCFB9BA60333488DBDD02D601B41AAE317CA7474EB6E6DD", "81111F140C7EADB97F10ACBFA7D9E5A7B65AA48C8116E86E136A7E9F209686E737D8470E1672DC400D8C943C24909AA9EEC7F8AE65E4208A00E8046A73A114E8" }
		, { 4127, "C33F4ACD6701A139FF1B19E61AF5D916C056722FE04ED799AF007142814F01D9790B2E1930AAFEE9675FDC721CA114A1BE15409DA17FE6E25334FBE90A1E57B88B06041C991F6DF84D7B72F620E429F20EC7D7B675B2E0587421ABA23909B8B586A702E32D6B3B63EBBB5D3A89189BBF091A2749A41369863603B62EE37E36BFB11980784DB47AED6DF9A8D72B8FA0CCE9A43AFA5516CFE5BB4F771832225D2C6C62F134458588B9CF75BBCDECF9FDF097B818EC3401010A5E36EFB49B83F5E7433628E6EEF82A18BA3AA265A810AF756B08D1A73EEDD1B33211FC8EC5A9CF337AB591DA8551E25C5BC85EBAD959B623E35C3DFE56B25032AA99B7543A52F639EFAA386F9773B22FE96EA3EBCA6177E263EB4905E5240915451514D8C75F6720EA0736E2D067B2D4AD40EBABEC6E5DFD7F5934F6EE612EBD7B7CFE398A48FA3F3C3136D688374439F9B40EB53E4BC908D535C5DB6059D203D79A306D2E07F03F4ECBD6A4E5789E9C9282AC137AC851D7D32CE3ABD3632E63095D6CED7CC50E2D497384BD1721DA7F709967CA1D5E4251E95BFFE32995FF30DCF9A39B291F3A3024BD1027C00BE8798A98B7BC111CF72F58678EAC07B442BFF1484B2F2131E217368AE40187BC7A69F70590731E2B67A081AD1EAB813A9E07E52AC0A29A74FAE5465E163EF3D290304392FF87CFCC55CB2ED931AF94D8AB11513DFA8884D1C917F0AB227E", "726E7066979EE647F06FBFCB9264F4103380BDE1A1BE6E561A1BE462F46F595FFDF7F4D4FBE6A5B25D744B20B588AEFC48014B359EEA8CE91B16E388322E2F9C" }
		, { 0, NULL }
	};

};

UNITTEST_SUITE_BEGIN(xskein)
{
	UNITTEST_FIXTURE(type)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}
	}

	UNITTEST_FIXTURE(generator)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Empty)
		{

		}

		UNITTEST_TEST(test1)
		{
			u8 message[] = { 0xFF };
			xskein256 hash = x_skein256Hash256(message, 1);

			u8 vhash[] =
			{
				0xA4, 0x7B, 0xE7, 0x1A, 0x18, 0x5B, 0xA0, 0xAF, 0x82, 0x0B, 0x3C, 0xE8, 0x45, 0xA3, 0xD3, 0x5A,
				0x80, 0xEC, 0x64, 0xF9, 0x6A, 0x0D, 0x6A, 0x36, 0xE3, 0xF5, 0x36, 0x36, 0x24, 0xD8, 0xA0, 0x91
			};

			for (u32 i = 0; i < hash.size(); ++i)
				CHECK_EQUAL(vhash[i], hash.data()[i]);
		}

		UNITTEST_TEST(test2)
		{
			u8 message[] = { 0xFB,0xD1,0x7C,0x26 };
			xskein256 hash = x_skein256Hash256(message, 4);

			u8 vhash[] =
			{
				0xBA,0x4D,0x44,0x62,0x35,0x18,0xD9,0x94,0xAD,0x01,0x9E,0xBD,0xB5,0x99,0x44,0xA9,
				0xFB,0x46,0x17,0xD8,0x59,0xD9,0x2E,0x33,0x82,0x88,0x9D,0xA4,0x4C,0xB8,0x05,0xA2
			};

			for (u32 i = 0; i < hash.size(); ++i)
				CHECK_EQUAL(vhash[i], hash.data()[i]);
		}

		UNITTEST_TEST(test_vectors)
		{
			xuchars128 digest_str;
			xbyte* bytemsg = SkeinTestVectors::ByteMsg;
			SkeinTestVectors::Vector* test = SkeinTestVectors::Tests;
			while (test->Msg != NULL)
			{
				u32 const test_bytelen = (test->Len + 7) / 8;
				u32 len = SkeinTestVectors::TextMsgToByteMsg(test->Msg, test_bytelen, bytemsg);
				CHECK_EQUAL(test_bytelen, len);
				xskein512 digest = x_skein512Hash512B(bytemsg, test->Len);
				digest.toString(digest_str.chars());

				s32 c = ascii::compare(ascii::crunes(test->Digest), digest_str.cchars());
				CHECK_EQUAL(0, c);
				test = test + 1;
			}
		}
	}
}
UNITTEST_SUITE_END
