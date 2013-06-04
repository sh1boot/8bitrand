#include <stddef.h>
#include "tables.h"

static const uint16_t lag_1_multipliers[] = {
    0x8037, /* 0x8036FF (8402687) is a safe prime */
    0x8214, /* 0x8213FF (8524799) is a safe prime */
    0x8943, /* 0x8942FF (8995583) is a safe prime */
    0x8c37, /* 0x8C36FF (9189119) is a safe prime */
    0x8f6a, /* 0x8F69FF (9398783) is a safe prime */
    0x933f, /* 0x933EFF (9649919) is a safe prime */
    0x9447, /* 0x9446FF (9717503) is a safe prime */
    0x9c0c, /* 0x9C0BFF (10226687) is a safe prime */
    0x9d47, /* 0x9D46FF (10307327) is a safe prime */
    0xa25d, /* 0xA25CFF (10640639) is a safe prime */
    0xa323, /* 0xA322FF (10691327) is a safe prime */
    0xa602, /* 0xA601FF (10879487) is a safe prime */
    0xa90b, /* 0xA90AFF (11078399) is a safe prime */
    0xb649, /* 0xB648FF (11946239) is a safe prime */
    0xb841, /* 0xB840FF (12075263) is a safe prime */
    0xc015, /* 0xC014FF (12588287) is a safe prime */
    0xc120, /* 0xC11FFF (12656639) is a safe prime */
    0xc72c, /* 0xC72BFF (13052927) is a safe prime */
    0xc813, /* 0xC812FF (13112063) is a safe prime */
    0xe118, /* 0xE117FF (14751743) is a safe prime */
    0xf609, /* 0xF608FF (16124159) is a safe prime */
};
static const uint16_t lag_2_multipliers[] = {
    0x8151, /* 0x8150FFFF (2169569279) is a safe prime */
    0x8466, /* 0x8465FFFF (2221277183) is a safe prime */
    0x856e, /* 0x856DFFFF (2238578687) is a safe prime */
    0x871e, /* 0x871DFFFF (2266890239) is a safe prime */
    0x884a, /* 0x8849FFFF (2286551039) is a safe prime */
    0x8b3b, /* 0x8B3AFFFF (2335899647) is a safe prime */
    0x9654, /* 0x9653FFFF (2522087423) is a safe prime */
    0x9c0c, /* 0x9C0BFFFF (2618032127) is a safe prime */
    0xa023, /* 0xA022FFFF (2686648319) is a safe prime */
    0xa341, /* 0xA340FFFF (2738946047) is a safe prime */
    0xa743, /* 0xA742FFFF (2806185983) is a safe prime */
    0xae18, /* 0xAE17FFFF (2920808447) is a safe prime */
    0xc039, /* 0xC038FFFF (3224961023) is a safe prime */
    0xc72c, /* 0xC72BFFFF (3341549567) is a safe prime */
    0xca11, /* 0xCA10FFFF (3390111743) is a safe prime */
    0xce07, /* 0xCE06FFFF (3456565247) is a safe prime */
    0xd722, /* 0xD721FFFF (3609329663) is a safe prime */
    0xe103, /* 0xE102FFFF (3775070207) is a safe prime */
};
static const uint16_t lag_3_multipliers[] = {
    0x9204, /* 0x9203FFFFFF (627132334079) is a safe prime */
    0x9c09, /* 0x9C08FFFFFF (670165893119) is a safe prime */
    0xa446, /* 0xA445FFFFFF (705549041663) is a safe prime */
    0xa54e, /* 0xA54DFFFFFF (709978226687) is a safe prime */
    0xb04f, /* 0xB04EFFFFFF (757239644159) is a safe prime */
    0xb21a, /* 0xB219FFFFFF (764940386303) is a safe prime */
    0xb91c, /* 0xB91BFFFFFF (795038711807) is a safe prime */
    0xc204, /* 0xC203FFFFFF (833290764287) is a safe prime */
    0xce0d, /* 0xCE0CFFFFFF (884981366783) is a safe prime */
};
static const uint16_t lag_4_multipliers[] = {
    0x8304, /* 0x8303FFFFFFFF (144053203107839) is a safe prime */
    0x8958, /* 0x8957FFFFFFFF (151011050127359) is a safe prime */
    0x8a2a, /* 0x8A29FFFFFFFF (151912993259519) is a safe prime */
    0x9018, /* 0x9017FFFFFFFF (158432753614847) is a safe prime */
    0x9768, /* 0x9767FFFFFFFF (166472932392959) is a safe prime */
    0x9c45, /* 0x9C44FFFFFFFF (171820166676479) is a safe prime */
    0x9e34, /* 0x9E33FFFFFFFF (173946175487999) is a safe prime */
    0xa52d, /* 0xA52CFFFFFFFF (181612692111359) is a safe prime */
    0xb211, /* 0xB210FFFFFFFF (195786084188159) is a safe prime */
    0xca17, /* 0xCA16FFFFFFFF (222200133058559) is a safe prime */
    0xcc09, /* 0xCC08FFFFFFFF (224339026771967) is a safe prime */
    0xdc23, /* 0xDC22FFFFFFFF (242042881966079) is a safe prime */
    0xde1b, /* 0xDE1AFFFFFFFF (244207545483263) is a safe prime */
    0xed0c, /* 0xED0BFFFFFFFF (260635795390463) is a safe prime */
};
static const uint16_t lag_5_multipliers[] = {
    0x8709, /* 0x8708FFFFFFFFFF (38009017460588543) is a safe prime */
    0x8b4a, /* 0x8B49FFFFFFFFFF (39206385623236607) is a safe prime */
    0x954c, /* 0x954BFFFFFFFFFF (42023334413598719) is a safe prime */
    0x9f24, /* 0x9F23FFFFFFFFFF (44794103715594239) is a safe prime */
    0xa347, /* 0xA346FFFFFFFFFF (45958486529409023) is a safe prime */
    0xb328, /* 0xB327FFFFFFFFFF (50428001296318463) is a safe prime */
    0xc10b, /* 0xC10AFFFFFFFFFF (54336765133062143) is a safe prime */
    0xd91a, /* 0xD919FFFFFFFFFF (61108657248534527) is a safe prime */
    0xdb09, /* 0xDB08FFFFFFFFFF (61652915504283647) is a safe prime */
    0xdc1d, /* 0xDC1CFFFFFFFFFF (61956380713549823) is a safe prime */
};
static const uint16_t lag_6_multipliers[] = {
    0x8331, /* 0x8330FFFFFFFFFFFF (9453337092827381759) is a safe prime */
    0x8c58, /* 0x8C57FFFFFFFFFFFF (10112832963260448767) is a safe prime */
    0xa25d, /* 0xA25CFFFFFFFFFFFF (11699507406978416639) is a safe prime */
    0xa440, /* 0xA43FFFFFFFFFFFFF (11835459820729663487) is a safe prime */
    0xd803, /* 0xD802FFFFFFFFFFFF (15565284737122566143) is a safe prime */
    0xe604, /* 0xE603FFFFFFFFFFFF (16574372528630267903) is a safe prime */
    0xee0b, /* 0xEE0AFFFFFFFFFFFF (17152803605770665983) is a safe prime */
};
static const uint16_t lag_7_multipliers[] = {
    0x8235, /* 0x8234FFFFFFFFFFFFFF (2401895782066251890687) is a safe prime */
    0x9513, /* 0x9512FFFFFFFFFFFFFF (2749933961269443821567) is a safe prime */
    0x9915, /* 0x9914FFFFFFFFFFFFFF (2823865052752357883903) is a safe prime */
    0x9966, /* 0x9965FFFFFFFFFFFFFF (2829701717869430046719) is a safe prime */
    0x9b37, /* 0x9B36FFFFFFFFFFFFFF (2863208499097066536959) is a safe prime */
    0x9d5c, /* 0x9D5BFFFFFFFFFFFFFF (2902768118223888973823) is a safe prime */
    0xa446, /* 0xA445FFFFFFFFFFFFFF (3030310059671021420543) is a safe prime */
    0xb50e, /* 0xB50DFFFFFFFFFFFFFF (3339869483657959833599) is a safe prime */
    0xb733, /* 0xB732FFFFFFFFFFFFFF (3379429102784782270463) is a safe prime */
    0xbc43, /* 0xBC42FFFFFFFFFFFFFF (3472815744657936875519) is a safe prime */
    0xbd18, /* 0xBD17FFFFFFFFFFFFFF (3488164012188015525887) is a safe prime */
    0xc813, /* 0xC812FFFFFFFFFFFFFF (3690717909028630953983) is a safe prime */
};
static const uint16_t lag_8_multipliers[] = {
    0x8d21, /* 0x8D20FFFFFFFFFFFFFFFF (666462416639052390334463) is a safe prime */
    0x9153, /* 0x9152FFFFFFFFFFFFFFFF (686274219774216448770047) is a safe prime */
    0x924f, /* 0x924EFFFFFFFFFFFFFFFF (690922799280791255777279) is a safe prime */
    0x970e, /* 0x970DFFFFFFFFFFFFFFFF (713335593330348360990719) is a safe prime */
    0xa00e, /* 0xA00DFFFFFFFFFFFFFFFF (755836891676175167913983) is a safe prime */
    0xba27, /* 0xBA26FFFFFFFFFFFFFFFF (879079588832628682260479) is a safe prime */
    0xdc17, /* 0xDC16FFFFFFFFFFFFFFFF (1039344901345017266700287) is a safe prime */
    0xf40b, /* 0xF40AFFFFFFFFFFFFFFFF (1152460336005004237209599) is a safe prime */
};
static const uint16_t lag_9_multipliers[] = {
    0x8154, /* 0x8153FFFFFFFFFFFFFFFFFF (156348109514848213735047167) is a safe prime */
    0x9216, /* 0x9215FFFFFFFFFFFFFFFFFF (176607061726358991701803007) is a safe prime */
    0x9414, /* 0x9413FFFFFFFFFFFFFFFFFF (179015468632622510760787967) is a safe prime */
    0x9933, /* 0x9932FFFFFFFFFFFFFFFFFF (185206491091664615635943423) is a safe prime */
};
static const uint16_t lag_10_multipliers[] = {
    0x9861, /* 0x9860FFFFFFFFFFFFFFFFFFFF (47158987297347069476113219583) is a safe prime */
    0xbc2e, /* 0xBC2DFFFFFFFFFFFFFFFFFFFF (58238792434115145862295322623) is a safe prime */
    0xd023, /* 0xD022FFFFFFFFFFFFFFFFFFFF (64415194446526286315869175807) is a safe prime */
    0xf906, /* 0xF905FFFFFFFFFFFFFFFFFFFF (77069021000432609887518719999) is a safe prime */
};
static const uint16_t lag_11_multipliers[] = {
    0x8136, /* 0x8135FFFFFFFFFFFFFFFFFFFFFF (10237145154870452183278307770367) is a safe prime */
    0xb124, /* 0xB123FFFFFFFFFFFFFFFFFFFFFF (14034526225378356176531371327487) is a safe prime */
};
static const uint16_t lag_12_multipliers[] = {
    0xae24, /* 0xAE23FFFFFFFFFFFFFFFFFFFFFFFF (3531991484885904169920189305978879) is a safe prime */
    0xb319, /* 0xB318FFFFFFFFFFFFFFFFFFFFFFFF (3632532023116505614326396578955263) is a safe prime */
    0xdb09, /* 0xDB08FFFFFFFFFFFFFFFFFFFFFFFF (4442560756662344201882789927190527) is a safe prime */
    0xfd02, /* 0xFD01FFFFFFFFFFFFFFFFFFFFFFFF (5131608086048901145933841663262719) is a safe prime */
};
static const uint16_t lag_13_multipliers[] = {
    0x8370, /* 0x836FFFFFFFFFFFFFFFFFFFFFFFFFFF (682462518343671406424977111271866367) is a safe prime */
    0xb112, /* 0xB111FFFFFFFFFFFFFFFFFFFFFFFFFF (919401627333530220317528900795105279) is a safe prime */
    0xe310, /* 0xE30FFFFFFFFFFFFFFFFFFFFFFFFFFF (1178975905441064298403205822753538047) is a safe prime */
    0xe802, /* 0xE801FFFFFFFFFFFFFFFFFFFFFFFFFF (1204653435999287313159923042881634303) is a safe prime */
};
static const uint16_t lag_14_multipliers[] = {
    0x6804, /* 0x6803FFFFFFFFFFFFFFFFFFFFFFFFFFFF (138260480749065390092510056254472716287) is a safe prime */
    0x7a52, /* 0x7A51FFFFFFFFFFFFFFFFFFFFFFFFFFFF (162591583828159592359803962053198086143) is a safe prime */
    0xd81b, /* 0xD81AFFFFFFFFFFFFFFFFFFFFFFFFFFFF (287253439104722268893192648421443371007) is a safe prime */
};
static const uint16_t lag_18_multipliers[] = {
    0x43b0, /* 0x43AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (386427312800138637796530926228450808175628648447) is a safe prime */
    0x4d9d, /* 0x4D9CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (443093506349604951199173186359250294762324885503) is a safe prime */
    0xb205, /* 0xB204FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (1016311860932636088429207288839403778912045498367) is a safe prime */
};
static const uint16_t lag_22_multipliers[] = {
    0x6b8e, /* 0x6B8DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (2637233263887586489127420440347288316359897686887706394623) is a safe prime */
    0x8232, /* 0x8231FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (3192379773566254728067731650932487818125785933898716282879) is a safe prime */
    0xc204, /* 0xC203FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (4757249282732935488558898759031347283248470920038447054847) is a safe prime */
};
static const uint16_t lag_26_multipliers[] = {
    0xc60c, /* 0xC60BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (20856770264046286584314234388914417650656177096905624767943619379199) is a safe prime */
    0x4614, /* 0x4613FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (7380087939585609099065036783769717014847570357366605687118511472639) is a safe prime */
    0x5211, /* 0x5210FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (8642601311190304434908436889086844189795574450273969837272731746303) is a safe prime */
    0x6e73, /* 0x6E72FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (11631660339564275210482938409202271382096714150197367659045480038399) is a safe prime */
    0x726f, /* 0x726EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (12051264001681182751232455550754395760867311795933930524199375339519) is a safe prime */
    0x7e75, /* 0x7E74FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (13317479758539850800670704336732276974451527044538970229046129983487) is a safe prime */
};
static const uint16_t lag_30_multipliers[] = {
    0x31a1, /* 0x31A0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (22447791958009372907355794746938779743588182909804738862935006787422734254079) is a safe prime */
    0x7164, /* 0x7163FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (51288036596386940319143959851565438677440202558505467116353984811122166857727) is a safe prime */
};
static const uint16_t small_lag_1_multipliers[] = {
    0x00ae, /* 0xADFF (44543) is a safe prime */
    0x00cc, /* 0xCBFF (52223) is a safe prime */
    0x00d2, /* 0xD1FF (53759) is a safe prime */
    0x0102, /* 0x0101FF (66047) is a safe prime */
};
static const uint16_t small_lag_2_multipliers[] = {
    0x0159, /* 0x0158FFFF (22609919) is a safe prime */
};
static const uint16_t small_lag_3_multipliers[] = {
    0x007b, /* 0x7AFFFFFF (2063597567) is a safe prime */
    0x00e4, /* 0xE3FFFFFF (3825205247) is a safe prime */
    0x013e, /* 0x013DFFFFFF (5335154687) is a safe prime */
};
static const uint16_t small_lag_5_multipliers[] = {
    0x0108, /* 0x0107FFFFFFFFFF (290271069732863) is a safe prime */
};
static const uint16_t small_lag_6_multipliers[] = {
    0x00ea, /* 0xE9FFFFFFFFFFFF (65865144550293503) is a safe prime */
    0x013b, /* 0x013AFFFFFFFFFFFF (88664617663856639) is a safe prime */
};
static const uint16_t small_lag_9_multipliers[] = {
    0x0102, /* 0x0101FFFFFFFFFFFFFFFFFF (1218370552580368465133567) is a safe prime */
};
static const uint16_t small_lag_10_multipliers[] = {
    0x0153, /* 0x0152FFFFFFFFFFFFFFFFFFFF (409825852849359290225393663) is a safe prime */
};
static const uint16_t small_lag_15_multipliers[] = {
    0x006c, /* 0x6BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (143556623544770914273611162510277214207) is a safe prime */
};

static const uint16_t unsafe_lag_1_multipliers[] = {
    0x0021, /* 0x20FF is prime */
    0x002d, /* 0x2CFF is prime */
    0x0035, /* 0x34FF is prime */
    0x003c, /* 0x3BFF is prime */
    0x003f, /* 0x3EFF is prime */
    0x0050, /* 0x4FFF is prime */
    0x0053, /* 0x52FF is prime */
    0x0054, /* 0x53FF is prime */
    0x0057, /* 0x56FF is prime */
    0x0059, /* 0x58FF is prime */
    0x005a, /* 0x59FF is prime */
    0x0062, /* 0x61FF is prime */
    0x0063, /* 0x62FF is prime */
    0x0069, /* 0x68FF is prime */
    0x006c, /* 0x6BFF is prime */
    0x007a, /* 0x79FF is prime */
    0x0081, /* 0x80FF is prime */
    0x0084, /* 0x83FF is prime */
    0x008a, /* 0x89FF is prime */
    0x008c, /* 0x8BFF is prime */
    0x008f, /* 0x8EFF is prime */
    0x009b, /* 0x9AFF is prime */
    0x00a4, /* 0xA3FF is prime */
    0x00a5, /* 0xA4FF is prime */
    0x00a7, /* 0xA6FF is prime */
    0x00b6, /* 0xB5FF is prime */
    0x00bd, /* 0xBCFF is prime */
    0x00c3, /* 0xC2FF is prime */
    0x00da, /* 0xD9FF is prime */
    0x00e3, /* 0xE2FF is prime */
    0x00e4, /* 0xE3FF is prime */
    0x00f3, /* 0xF2FF is prime */
    0x00f9, /* 0xF8FF is prime */
    0x010d, /* 0x010CFF is prime */
    0x010e, /* 0x010DFF is prime */
    0x0116, /* 0x0115FF is prime */
    0x011d, /* 0x011CFF is prime */
    0x0120, /* 0x011FFF is prime */
    0x0129, /* 0x0128FF is prime */
    0x0131, /* 0x0130FF is prime */
    0x0135, /* 0x0134FF is prime */
    0x0140, /* 0x013FFF is prime */
    0x0149, /* 0x0148FF is prime */
    0x014d, /* 0x014CFF is prime */
    0x0153, /* 0x0152FF is prime */
    0x015c, /* 0x015BFF is prime */
    0x016b, /* 0x016AFF is prime */
    0x0170, /* 0x016FFF is prime */
    0x0171, /* 0x0170FF is prime */
    0x0186, /* 0x0185FF is prime */
    0x018b, /* 0x018AFF is prime */
    0x0192, /* 0x0191FF is prime */
    0x0194, /* 0x0193FF is prime */
    0x019a, /* 0x0199FF is prime */
    0x019d, /* 0x019CFF is prime */
    0x019e, /* 0x019DFF is prime */
    0x01a7, /* 0x01A6FF is prime */
    0x01a9, /* 0x01A8FF is prime */
    0x01ac, /* 0x01ABFF is prime */
    0x01b2, /* 0x01B1FF is prime */
    0x01cb, /* 0x01CAFF is prime */
    0x01d1, /* 0x01D0FF is prime */
    0x01da, /* 0x01D9FF is prime */
    0x01e9, /* 0x01E8FF is prime */
    0x01ef, /* 0x01EEFF is prime */
    0x01f2, /* 0x01F1FF is prime */
    0x01f7, /* 0x01F6FF is prime */
    0x01f8, /* 0x01F7FF is prime */
};
static const uint16_t unsafe_lag_2_multipliers[] = {
    0x002d, /* 0x2CFFFF is prime */
    0x003b, /* 0x3AFFFF is prime */
    0x003f, /* 0x3EFFFF is prime */
    0x0075, /* 0x74FFFF is prime */
    0x0077, /* 0x76FFFF is prime */
    0x0081, /* 0x80FFFF is prime */
    0x0087, /* 0x86FFFF is prime */
    0x008a, /* 0x89FFFF is prime */
    0x0095, /* 0x94FFFF is prime */
    0x00a2, /* 0xA1FFFF is prime */
    0x00a4, /* 0xA3FFFF is prime */
    0x00ad, /* 0xACFFFF is prime */
    0x00b6, /* 0xB5FFFF is prime */
    0x00d7, /* 0xD6FFFF is prime */
    0x00e0, /* 0xDFFFFF is prime */
    0x0101, /* 0x0100FFFF is prime */
    0x010b, /* 0x010AFFFF is prime */
    0x0110, /* 0x010FFFFF is prime */
    0x011f, /* 0x011EFFFF is prime */
    0x0120, /* 0x011FFFFF is prime */
    0x012c, /* 0x012BFFFF is prime */
    0x014c, /* 0x014BFFFF is prime */
    0x0152, /* 0x0151FFFF is prime */
    0x016a, /* 0x0169FFFF is prime */
    0x0176, /* 0x0175FFFF is prime */
    0x017f, /* 0x017EFFFF is prime */
    0x0183, /* 0x0182FFFF is prime */
    0x0192, /* 0x0191FFFF is prime */
    0x0194, /* 0x0193FFFF is prime */
    0x0198, /* 0x0197FFFF is prime */
    0x019e, /* 0x019DFFFF is prime */
    0x01ad, /* 0x01ACFFFF is prime */
    0x01b5, /* 0x01B4FFFF is prime */
    0x01d1, /* 0x01D0FFFF is prime */
    0x01d4, /* 0x01D3FFFF is prime */
    0x01da, /* 0x01D9FFFF is prime */
    0x01e2, /* 0x01E1FFFF is prime */
    0x01e8, /* 0x01E7FFFF is prime */
    0x01ef, /* 0x01EEFFFF is prime */
};
static const uint16_t unsafe_lag_3_multipliers[] = {
    0x0027, /* 0x26FFFFFF is prime */
    0x002c, /* 0x2BFFFFFF is prime */
    0x005d, /* 0x5CFFFFFF is prime */
    0x0072, /* 0x71FFFFFF is prime */
    0x0075, /* 0x74FFFFFF is prime */
    0x0087, /* 0x86FFFFFF is prime */
    0x009f, /* 0x9EFFFFFF is prime */
    0x00a8, /* 0xA7FFFFFF is prime */
    0x00bd, /* 0xBCFFFFFF is prime */
    0x00c6, /* 0xC5FFFFFF is prime */
    0x00d2, /* 0xD1FFFFFF is prime */
    0x00f3, /* 0xF2FFFFFF is prime */
    0x00ff, /* 0xFEFFFFFF is prime */
    0x0126, /* 0x0125FFFFFF is prime */
    0x0129, /* 0x0128FFFFFF is prime */
    0x012f, /* 0x012EFFFFFF is prime */
    0x0147, /* 0x0146FFFFFF is prime */
    0x0156, /* 0x0155FFFFFF is prime */
    0x0162, /* 0x0161FFFFFF is prime */
    0x018f, /* 0x018EFFFFFF is prime */
    0x01b0, /* 0x01AFFFFFFF is prime */
    0x01bc, /* 0x01BBFFFFFF is prime */
    0x01c2, /* 0x01C1FFFFFF is prime */
    0x01cb, /* 0x01CAFFFFFF is prime */
    0x01da, /* 0x01D9FFFFFF is prime */
};
static const uint16_t unsafe_lag_4_multipliers[] = {
    0x0039, /* 0x38FFFFFFFF is prime */
    0x003f, /* 0x3EFFFFFFFF is prime */
    0x0057, /* 0x56FFFFFFFF is prime */
    0x0069, /* 0x68FFFFFFFF is prime */
    0x008f, /* 0x8EFFFFFFFF is prime */
    0x00a7, /* 0xA6FFFFFFFF is prime */
    0x00ad, /* 0xACFFFFFFFF is prime */
    0x00c0, /* 0xBFFFFFFFFF is prime */
    0x0108, /* 0x0107FFFFFFFF is prime */
    0x0110, /* 0x010FFFFFFFFF is prime */
    0x0126, /* 0x0125FFFFFFFF is prime */
    0x012b, /* 0x012AFFFFFFFF is prime */
    0x0135, /* 0x0134FFFFFFFF is prime */
    0x013a, /* 0x0139FFFFFFFF is prime */
    0x0149, /* 0x0148FFFFFFFF is prime */
    0x017a, /* 0x0179FFFFFFFF is prime */
    0x01e3, /* 0x01E2FFFFFFFF is prime */
    0x01e9, /* 0x01E8FFFFFFFF is prime */
};
static const uint16_t unsafe_lag_5_multipliers[] = {
    0x0026, /* 0x25FFFFFFFFFF is prime */
    0x0048, /* 0x47FFFFFFFFFF is prime */
    0x006b, /* 0x6AFFFFFFFFFF is prime */
    0x006e, /* 0x6DFFFFFFFFFF is prime */
    0x0075, /* 0x74FFFFFFFFFF is prime */
    0x0084, /* 0x83FFFFFFFFFF is prime */
    0x0093, /* 0x92FFFFFFFFFF is prime */
    0x00b9, /* 0xB8FFFFFFFFFF is prime */
    0x00c5, /* 0xC4FFFFFFFFFF is prime */
    0x00d1, /* 0xD0FFFFFFFFFF is prime */
    0x00d4, /* 0xD3FFFFFFFFFF is prime */
    0x00e0, /* 0xDFFFFFFFFFFF is prime */
    0x00ff, /* 0xFEFFFFFFFFFF is prime */
    0x010b, /* 0x010AFFFFFFFFFF is prime */
    0x0113, /* 0x0112FFFFFFFFFF is prime */
    0x0116, /* 0x0115FFFFFFFFFF is prime */
    0x013e, /* 0x013DFFFFFFFFFF is prime */
    0x014c, /* 0x014BFFFFFFFFFF is prime */
    0x0159, /* 0x0158FFFFFFFFFF is prime */
    0x015e, /* 0x015DFFFFFFFFFF is prime */
    0x0165, /* 0x0164FFFFFFFFFF is prime */
    0x0183, /* 0x0182FFFFFFFFFF is prime */
    0x01b5, /* 0x01B4FFFFFFFFFF is prime */
    0x01f2, /* 0x01F1FFFFFFFFFF is prime */
    0x01fe, /* 0x01FDFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_6_multipliers[] = {
    0x0026, /* 0x25FFFFFFFFFFFF is prime */
    0x002c, /* 0x2BFFFFFFFFFFFF is prime */
    0x0059, /* 0x58FFFFFFFFFFFF is prime */
    0x0075, /* 0x74FFFFFFFFFFFF is prime */
    0x008a, /* 0x89FFFFFFFFFFFF is prime */
    0x0095, /* 0x94FFFFFFFFFFFF is prime */
    0x0096, /* 0x95FFFFFFFFFFFF is prime */
    0x00a4, /* 0xA3FFFFFFFFFFFF is prime */
    0x00a8, /* 0xA7FFFFFFFFFFFF is prime */
    0x00b1, /* 0xB0FFFFFFFFFFFF is prime */
    0x00bd, /* 0xBCFFFFFFFFFFFF is prime */
    0x00d4, /* 0xD3FFFFFFFFFFFF is prime */
    0x00e6, /* 0xE5FFFFFFFFFFFF is prime */
    0x00e9, /* 0xE8FFFFFFFFFFFF is prime */
    0x0116, /* 0x0115FFFFFFFFFFFF is prime */
    0x0129, /* 0x0128FFFFFFFFFFFF is prime */
    0x0140, /* 0x013FFFFFFFFFFFFF is prime */
    0x015b, /* 0x015AFFFFFFFFFFFF is prime */
    0x015c, /* 0x015BFFFFFFFFFFFF is prime */
    0x0180, /* 0x017FFFFFFFFFFFFF is prime */
    0x019a, /* 0x0199FFFFFFFFFFFF is prime */
    0x019d, /* 0x019CFFFFFFFFFFFF is prime */
    0x01c5, /* 0x01C4FFFFFFFFFFFF is prime */
    0x01ce, /* 0x01CDFFFFFFFFFFFF is prime */
    0x01df, /* 0x01DEFFFFFFFFFFFF is prime */
    0x01e9, /* 0x01E8FFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_7_multipliers[] = {
    0x0021, /* 0x20FFFFFFFFFFFFFF is prime */
    0x0066, /* 0x65FFFFFFFFFFFFFF is prime */
    0x006e, /* 0x6DFFFFFFFFFFFFFF is prime */
    0x0081, /* 0x80FFFFFFFFFFFFFF is prime */
    0x009e, /* 0x9DFFFFFFFFFFFFFF is prime */
    0x00a5, /* 0xA4FFFFFFFFFFFFFF is prime */
    0x00ae, /* 0xADFFFFFFFFFFFFFF is prime */
    0x00b6, /* 0xB5FFFFFFFFFFFFFF is prime */
    0x00b7, /* 0xB6FFFFFFFFFFFFFF is prime */
    0x00bd, /* 0xBCFFFFFFFFFFFFFF is prime */
    0x00ea, /* 0xE9FFFFFFFFFFFFFF is prime */
    0x00f3, /* 0xF2FFFFFFFFFFFFFF is prime */
    0x0110, /* 0x010FFFFFFFFFFFFFFF is prime */
    0x0117, /* 0x0116FFFFFFFFFFFFFF is prime */
    0x0129, /* 0x0128FFFFFFFFFFFFFF is prime */
    0x0134, /* 0x0133FFFFFFFFFFFFFF is prime */
    0x0143, /* 0x0142FFFFFFFFFFFFFF is prime */
    0x014c, /* 0x014BFFFFFFFFFFFFFF is prime */
    0x014d, /* 0x014CFFFFFFFFFFFFFF is prime */
    0x015e, /* 0x015DFFFFFFFFFFFFFF is prime */
    0x0168, /* 0x0167FFFFFFFFFFFFFF is prime */
    0x0192, /* 0x0191FFFFFFFFFFFFFF is prime */
    0x01bc, /* 0x01BBFFFFFFFFFFFFFF is prime */
    0x01bf, /* 0x01BEFFFFFFFFFFFFFF is prime */
    0x01ce, /* 0x01CDFFFFFFFFFFFFFF is prime */
    0x01f2, /* 0x01F1FFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_8_multipliers[] = {
    0x004d, /* 0x4CFFFFFFFFFFFFFFFF is prime */
    0x0053, /* 0x52FFFFFFFFFFFFFFFF is prime */
    0x0059, /* 0x58FFFFFFFFFFFFFFFF is prime */
    0x0086, /* 0x85FFFFFFFFFFFFFFFF is prime */
    0x00c6, /* 0xC5FFFFFFFFFFFFFFFF is prime */
    0x00d2, /* 0xD1FFFFFFFFFFFFFFFF is prime */
    0x00d7, /* 0xD6FFFFFFFFFFFFFFFF is prime */
    0x00fc, /* 0xFBFFFFFFFFFFFFFFFF is prime */
    0x011f, /* 0x011EFFFFFFFFFFFFFFFF is prime */
    0x0158, /* 0x0157FFFFFFFFFFFFFFFF is prime */
    0x0161, /* 0x0160FFFFFFFFFFFFFFFF is prime */
    0x017c, /* 0x017BFFFFFFFFFFFFFFFF is prime */
    0x0186, /* 0x0185FFFFFFFFFFFFFFFF is prime */
    0x018e, /* 0x018DFFFFFFFFFFFFFFFF is prime */
    0x01c2, /* 0x01C1FFFFFFFFFFFFFFFF is prime */
    0x01e3, /* 0x01E2FFFFFFFFFFFFFFFF is prime */
    0x01e8, /* 0x01E7FFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_9_multipliers[] = {
    0x0030, /* 0x2FFFFFFFFFFFFFFFFFFF is prime */
    0x003b, /* 0x3AFFFFFFFFFFFFFFFFFF is prime */
    0x0054, /* 0x53FFFFFFFFFFFFFFFFFF is prime */
    0x0081, /* 0x80FFFFFFFFFFFFFFFFFF is prime */
    0x00b6, /* 0xB5FFFFFFFFFFFFFFFFFF is prime */
    0x00c8, /* 0xC7FFFFFFFFFFFFFFFFFF is prime */
    0x00e6, /* 0xE5FFFFFFFFFFFFFFFFFF is prime */
    0x0113, /* 0x0112FFFFFFFFFFFFFFFFFF is prime */
    0x013b, /* 0x013AFFFFFFFFFFFFFFFFFF is prime */
    0x0156, /* 0x0155FFFFFFFFFFFFFFFFFF is prime */
    0x0159, /* 0x0158FFFFFFFFFFFFFFFFFF is prime */
    0x016b, /* 0x016AFFFFFFFFFFFFFFFFFF is prime */
    0x01b8, /* 0x01B7FFFFFFFFFFFFFFFFFF is prime */
    0x01c4, /* 0x01C3FFFFFFFFFFFFFFFFFF is prime */
    0x01ce, /* 0x01CDFFFFFFFFFFFFFFFFFF is prime */
    0x01d0, /* 0x01CFFFFFFFFFFFFFFFFFFF is prime */
    0x01df, /* 0x01DEFFFFFFFFFFFFFFFFFF is prime */
    0x01e8, /* 0x01E7FFFFFFFFFFFFFFFFFF is prime */
    0x01f1, /* 0x01F0FFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_10_multipliers[] = {
    0x003c, /* 0x3BFFFFFFFFFFFFFFFFFFFF is prime */
    0x0062, /* 0x61FFFFFFFFFFFFFFFFFFFF is prime */
    0x0071, /* 0x70FFFFFFFFFFFFFFFFFFFF is prime */
    0x00a2, /* 0xA1FFFFFFFFFFFFFFFFFFFF is prime */
    0x00ad, /* 0xACFFFFFFFFFFFFFFFFFFFF is prime */
    0x00ae, /* 0xADFFFFFFFFFFFFFFFFFFFF is prime */
    0x00b6, /* 0xB5FFFFFFFFFFFFFFFFFFFF is prime */
    0x00d1, /* 0xD0FFFFFFFFFFFFFFFFFFFF is prime */
    0x00d5, /* 0xD4FFFFFFFFFFFFFFFFFFFF is prime */
    0x00e3, /* 0xE2FFFFFFFFFFFFFFFFFFFF is prime */
    0x010b, /* 0x010AFFFFFFFFFFFFFFFFFFFF is prime */
    0x012b, /* 0x012AFFFFFFFFFFFFFFFFFFFF is prime */
    0x014a, /* 0x0149FFFFFFFFFFFFFFFFFFFF is prime */
    0x0171, /* 0x0170FFFFFFFFFFFFFFFFFFFF is prime */
    0x0195, /* 0x0194FFFFFFFFFFFFFFFFFFFF is prime */
    0x01b6, /* 0x01B5FFFFFFFFFFFFFFFFFFFF is prime */
    0x01f7, /* 0x01F6FFFFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_11_multipliers[] = {
    0x0026, /* 0x25FFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0081, /* 0x80FFFFFFFFFFFFFFFFFFFFFF is prime */
    0x008f, /* 0x8EFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00b7, /* 0xB6FFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00c0, /* 0xBFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00ed, /* 0xECFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0108, /* 0x0107FFFFFFFFFFFFFFFFFFFFFF is prime */
    0x016b, /* 0x016AFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0183, /* 0x0182FFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0195, /* 0x0194FFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01ac, /* 0x01ABFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01da, /* 0x01D9FFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01f8, /* 0x01F7FFFFFFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_12_multipliers[] = {
    0x0048, /* 0x47FFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0059, /* 0x58FFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0066, /* 0x65FFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00ad, /* 0xACFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00f5, /* 0xF4FFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00fc, /* 0xFBFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x013a, /* 0x0139FFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0180, /* 0x017FFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01ca, /* 0x01C9FFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01d1, /* 0x01D0FFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0201, /* 0x0200FFFFFFFFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_13_multipliers[] = {
    0x0032, /* 0x31FFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x004e, /* 0x4DFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0084, /* 0x83FFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00ad, /* 0xACFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00b7, /* 0xB6FFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00c8, /* 0xC7FFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0102, /* 0x0101FFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0120, /* 0x011FFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0185, /* 0x0184FFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x018f, /* 0x018EFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01e3, /* 0x01E2FFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_14_multipliers[] = {
    0x008c, /* 0x8BFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00a4, /* 0xA3FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00aa, /* 0xA9FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00c5, /* 0xC4FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00d2, /* 0xD1FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00f0, /* 0xEFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00f8, /* 0xF7FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00fe, /* 0xFDFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x012b, /* 0x012AFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0143, /* 0x0142FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0171, /* 0x0170FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0189, /* 0x0188FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01c1, /* 0x01C0FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01c7, /* 0x01C6FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01d3, /* 0x01D2FFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_15_multipliers[] = {
    0x0036, /* 0x35FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x006c, /* 0x6BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0080, /* 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x008a, /* 0x89FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0099, /* 0x98FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00a4, /* 0xA3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0110, /* 0x010FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0134, /* 0x0133FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x013a, /* 0x0139FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x015c, /* 0x015BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x017f, /* 0x017EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01e0, /* 0x01DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01ec, /* 0x01EBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_16_multipliers[] = {
    0x005f, /* 0x5EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00e6, /* 0xE5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x010b, /* 0x010AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x019a, /* 0x0199FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01b8, /* 0x01B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01df, /* 0x01DEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_17_multipliers[] = {
    0x004b, /* 0x4AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0152, /* 0x0151FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x015c, /* 0x015BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0171, /* 0x0170FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x017f, /* 0x017EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x0180, /* 0x017FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01ec, /* 0x01EBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
};
static const uint16_t unsafe_lag_18_multipliers[] = {
    0x0050, /* 0x4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x008c, /* 0x8BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x009e, /* 0x9DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x00d5, /* 0xD4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x012b, /* 0x012AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01a9, /* 0x01A8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01bc, /* 0x01BBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01d0, /* 0x01CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
    0x01e8, /* 0x01E7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF is prime */
};

const tabptr_t multiplier_table[][SEED_MAX + 1] =
{
    [WIDE_TABLE] = {
        { 0, NULL },
        { sizeof(lag_1_multipliers) >> 1, lag_1_multipliers },
        { sizeof(lag_2_multipliers) >> 1, lag_2_multipliers },
        { sizeof(lag_3_multipliers) >> 1, lag_3_multipliers },
        { sizeof(lag_4_multipliers) >> 1, lag_4_multipliers },
        { sizeof(lag_5_multipliers) >> 1, lag_5_multipliers },
        { sizeof(lag_6_multipliers) >> 1, lag_6_multipliers },
        { sizeof(lag_7_multipliers) >> 1, lag_7_multipliers },
        { sizeof(lag_8_multipliers) >> 1, lag_8_multipliers },
        { sizeof(lag_9_multipliers) >> 1, lag_9_multipliers },
        { sizeof(lag_10_multipliers) >> 1, lag_10_multipliers },
        { sizeof(lag_11_multipliers) >> 1, lag_11_multipliers },
        { sizeof(lag_12_multipliers) >> 1, lag_12_multipliers },
        { sizeof(lag_13_multipliers) >> 1, lag_13_multipliers },
        { sizeof(lag_14_multipliers) >> 1, lag_14_multipliers },
        { 0, NULL },
        { 0, NULL },
        { 0, NULL },
        { sizeof(lag_18_multipliers) >> 1, lag_18_multipliers },
        { 0, NULL },
        { 0, NULL },
        { 0, NULL },
        { sizeof(lag_22_multipliers) >> 1, lag_22_multipliers },
        { 0, NULL },
        { 0, NULL },
        { 0, NULL },
        { sizeof(lag_26_multipliers) >> 1, lag_26_multipliers },
        { 0, NULL },
        { 0, NULL },
        { 0, NULL },
        { sizeof(lag_30_multipliers) >> 1, lag_30_multipliers },
    },
    [NARROW_TABLE] = {
        { 0, NULL },
        { sizeof(small_lag_1_multipliers) >> 1, small_lag_1_multipliers },
        { sizeof(small_lag_2_multipliers) >> 1, small_lag_2_multipliers },
        { sizeof(small_lag_3_multipliers) >> 1, small_lag_3_multipliers },
        { 0, NULL },
        { sizeof(small_lag_5_multipliers) >> 1, small_lag_5_multipliers },
        { sizeof(small_lag_6_multipliers) >> 1, small_lag_6_multipliers },
        { 0, NULL },
        { 0, NULL },
        { sizeof(small_lag_9_multipliers) >> 1, small_lag_9_multipliers },
        { sizeof(small_lag_10_multipliers) >> 1, small_lag_10_multipliers },
        { 0, NULL },
        { 0, NULL },
        { 0, NULL },
        { 0, NULL },
        { sizeof(small_lag_15_multipliers) >> 1, small_lag_15_multipliers },
    },
    [UNSAFE_TABLE] = {
        { 0, NULL },
        { sizeof(unsafe_lag_1_multipliers) >> 1, unsafe_lag_1_multipliers },
        { sizeof(unsafe_lag_2_multipliers) >> 1, unsafe_lag_2_multipliers },
        { sizeof(unsafe_lag_3_multipliers) >> 1, unsafe_lag_3_multipliers },
        { sizeof(unsafe_lag_4_multipliers) >> 1, unsafe_lag_4_multipliers },
        { sizeof(unsafe_lag_5_multipliers) >> 1, unsafe_lag_5_multipliers },
        { sizeof(unsafe_lag_6_multipliers) >> 1, unsafe_lag_6_multipliers },
        { sizeof(unsafe_lag_7_multipliers) >> 1, unsafe_lag_7_multipliers },
        { sizeof(unsafe_lag_8_multipliers) >> 1, unsafe_lag_8_multipliers },
        { sizeof(unsafe_lag_9_multipliers) >> 1, unsafe_lag_9_multipliers },
        { sizeof(unsafe_lag_10_multipliers) >> 1, unsafe_lag_10_multipliers },
        { sizeof(unsafe_lag_11_multipliers) >> 1, unsafe_lag_11_multipliers },
        { sizeof(unsafe_lag_12_multipliers) >> 1, unsafe_lag_12_multipliers },
        { sizeof(unsafe_lag_13_multipliers) >> 1, unsafe_lag_13_multipliers },
        { sizeof(unsafe_lag_14_multipliers) >> 1, unsafe_lag_14_multipliers },
        { sizeof(unsafe_lag_15_multipliers) >> 1, unsafe_lag_15_multipliers },
        { sizeof(unsafe_lag_16_multipliers) >> 1, unsafe_lag_16_multipliers },
        { sizeof(unsafe_lag_17_multipliers) >> 1, unsafe_lag_17_multipliers },
        { sizeof(unsafe_lag_18_multipliers) >> 1, unsafe_lag_18_multipliers },
    }
};
