#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/sysinfo.h>

#include "vendor_init.h"

char const *heapminfree;
char const *heapmaxfree;
char const *heaptargetutilization;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_4x(char const prop0[], char const prop1[], char const prop2[], char const prop3[], char const value[])
{
    property_override(prop0, value);
    property_override(prop1, value);
    property_override(prop2, value);
    property_override(prop3, value);
}

void property_override_9x(char const prop0[], char const prop1[], char const prop2[], char const prop3[], char const prop4[], char const prop5[], char const prop6[], char const prop7[], char const prop8[], char const value[])
{
    property_override_4x(prop0, prop1, prop2, prop3, value);
    property_override_4x(prop4, prop5, prop6, prop7, value);
    property_override(prop8, value);
}

static void set_model(const char *model) {
    property_override_4x("ro.build.product", "ro.product.device", "ro.product.model", "ro.product.name", model);
}

void check_device()
{
    struct sysinfo sys;

    sysinfo(&sys);

    if (sys.totalram > 3072ull * 1024 * 1024) {
        heaptargetutilization = "0.6";
        heapminfree = "4m";
        heapmaxfree = "16m";
    } else {
        heaptargetutilization = "0.75";
        heapminfree = "512k";
        heapmaxfree = "8m";
    }
}

void vendor_load_properties()
{
    int i;
    std::ifstream fin;
    std::string buf;

    fin.open("/sys/firmware/devicetree/base/hisi,boardname");
    while (std::getline(fin, buf, ' '))
        if ((buf.find("BTV_DL09") != std::string::npos) || (buf.find("BTV_L0J") != std::string::npos) || (buf.find("BTV_W09") != std::string::npos))
            break;
    fin.close();

    if (buf.find("BTV_DL09") != std::string::npos) {
	set_model("BTV-DL09");
	property_override("ro.build.description", "BTV-DL09-user 7.0 HUAWEIBEETHOVEN-DL09 C100B311 release-keys");
	property_override_9x("ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "ro.odm.build.fingerprint", "ro.bootimage.build.fingerprint", "ro.build.fingerprint", "ro.product.build.fingerprint", "ro.system_ext.build.fingerprint",  "ro.odm_dlkm.build.fingerprint", "ro.vendor_dlkm.build.fingerprint",  "HUAWEI/BEETHOVEN/hwbeethoven:7.0/HUAWEIBEETHOVEN-DL09/C100B311:user/release-keys");	
    }
    else if (buf.find("BTV_L0J") != std::string::npos) {
	set_model("BTV-L0J");
	property_override("ro.build.description", "BTV-L0J-user 7.0 HUAWEIBTV-L0J C137B365 release-keys");
	property_override_9x("ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "ro.odm.build.fingerprint", "ro.bootimage.build.fingerprint", "ro.build.fingerprint", "ro.product.build.fingerprint", "ro.system_ext.build.fingerprint",  "ro.odm_dlkm.build.fingerprint", "ro.vendor_dlkm.build.fingerprint",  "dtab/BEETHOVEN/d-01J:7.0/HUAWEIBTV-L0J/19021102:user/release-keys");
    }
    else if (buf.find("BTV_W09") != std::string::npos) {
	set_model("BTV-W09");
	property_override("persist.radio.noril", "1");
	property_override("ro.carrier", "wifi-only");
	property_override("ro.build.description", "BTV-W09-user 7.0 HUAWEIBEETHOVEN-W09 C100B308 release-keys");
	property_override_9x("ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "ro.odm.build.fingerprint", "ro.bootimage.build.fingerprint", "ro.build.fingerprint", "ro.product.build.fingerprint", "ro.system_ext.build.fingerprint",  "ro.odm_dlkm.build.fingerprint", "ro.vendor_dlkm.build.fingerprint",  "HUAWEI/BEETHOVEN/hwbeethoven:7.0/HUAWEIBEETHOVEN-W09/C100B308:user/release-keys");
    }
    else {
		set_model("UNKNOWN");
    }
    
    property_override("ro.connectivity.chiptype", "hisi");
    property_override("is_hisi_connectivity_chip", "1");
    property_override("ro.boot.odm.conn.chiptype", "hisi");
    property_override("ro.connectivity.sub_chiptype", "hi1102");
    property_override("ro.boot.odm.conn.schiptype", "hi1102");
    
    property_override("wifi.interface", "wlan0");
    property_override("wifi.direct.interface", "p2p-dev-wlan0");
    
    check_device();
    property_override("dalvik.vm.heapstartsize", "8m");
    property_override("dalvik.vm.heapgrowthlimit", "256m");
    property_override("dalvik.vm.heapsize", "512m");
    property_override("dalvik.vm.heaptargetutilization", heaptargetutilization);
    property_override("dalvik.vm.heapminfree", heapminfree);
    property_override("dalvik.vm.heapmaxfree", heapmaxfree);
}
