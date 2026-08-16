#pragma once
#include "base/PlumbumBase.hpp"
namespace Plumbum::components::route
{
    const inline QvConfig_Route emptyScheme;
    const inline QvConfig_Route noAdsScheme({ {}, { "geosite:category-ads-all" }, {} }, { {}, {}, {} }, "AsIs");

    /**
     * @brief The PlumbumRouteScheme struct
     * @author DuckSoft <realducksoft@gmail.com>
     */
    struct PlumbumRouteScheme : QvConfig_Route
    {
        /**
         * @brief the name of the scheme.
         * @example "Untitled Scheme"
         */
        QString name;
        /**
         * @brief the author of the scheme.
         * @example "DuckSoft <realducksoft@gmail.com>"
         */
        QString author;
        /**
         * @brief details of this scheme.
         * @example "A scheme to bypass China mainland, while allowing bilibili to go through proxy."
         */
        QString description;

        // M: all these fields are mandatory
        JSONSTRUCT_REGISTER(PlumbumRouteScheme, F(name, author, description), B(QvConfig_Route));
    };
} // namespace Plumbum::components::route

using namespace Plumbum::components::route;
