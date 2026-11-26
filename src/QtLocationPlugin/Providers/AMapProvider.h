#pragma once

#include <QtCore/QObject>

#include "MapProvider.h"

static constexpr const quint32 AVERAGE_AMAP_ROAD_MAP = QGC_AVERAGE_TILE_SIZE;
static constexpr const quint32 AVERAGE_AMAP_SAT_MAP   = QGC_AVERAGE_TILE_SIZE;

// 高德（AMap）离线瓦片图源。免 API Key，大陆境内可直连
//   街道图：http://webrd0{1..4}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=7&x={x}&y={y}&z={z}
//   卫星图：http://webrd0{1..4}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=6&x={x}&y={y}&z={z}
// 注：高德瓦片采用 GCJ-02 坐标系，而 QGC 其余管线（MAVLink GPS、QGeoTiled 投影）使用 WGS-84；
// 两者在国内会有数百米偏移，先期以"能打开地图"为目标，GCJ-02 偏移校正作为可选开关留待后续。
class AMapProvider : public MapProvider
{
protected:
    AMapProvider(const QString &mapName, const QString &referrer, const QString &imageFormat, quint32 averageSize,
                 MapProvider::MapStyle mapStyle, const QString &styleCode)
        : MapProvider(mapName, referrer, imageFormat, averageSize, mapStyle), _styleCode(styleCode) {}

private:
    QString _getURL(int x, int y, int zoom) const final;

    const QString _styleCode;
};

class AMapRoadProvider : public AMapProvider
{
public:
    AMapRoadProvider()
        : AMapProvider(
            QObject::tr("AMap Road"),
            QStringLiteral("https://www.amap.com/"),
            QStringLiteral("png"),
            AVERAGE_AMAP_ROAD_MAP,
            MapProvider::StreetMap,
            QStringLiteral("7")) {}
};

class AMapSatelliteProvider : public AMapProvider
{
public:
    AMapSatelliteProvider()
        : AMapProvider(
            QObject::tr("AMap Satellite"),
            QStringLiteral("https://www.amap.com/"),
            QStringLiteral("jpg"),
            AVERAGE_AMAP_SAT_MAP,
            MapProvider::SatelliteMapDay,
            QStringLiteral("6")) {}
};
