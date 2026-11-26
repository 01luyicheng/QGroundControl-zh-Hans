#include "AMapProvider.h"

QString AMapProvider::_getURL(int x, int y, int zoom) const
{
    // http://webrd{01-04}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style={6|7}&x={x}&y={y}&z={z}
    // 无需 API Key，高德官方免费瓦片服务；style=7 街道图，style=6 卫星图
    const QString url = QStringLiteral("http://webrd0%1.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=%2&x=%3&y=%4&z=%5");
    return url.arg(_getServerNum(x, y, 4) + 1).arg(_styleCode).arg(x).arg(y).arg(zoom);
}
