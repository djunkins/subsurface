// SPDX-License-Identifier: GPL-2.0
#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct divecomputer;
struct device;
struct device_table;
struct dive_table;

// global device table
extern struct device_table device_table;

extern void fake_dc(struct divecomputer *dc);
extern void set_dc_deviceid(struct divecomputer *dc, unsigned int deviceid, const struct device_table *table);

extern void set_dc_nickname(struct dive *dive, struct device_table *table);
extern void create_device_node(struct device_table *table, const char *model, uint32_t deviceid, const char *serial, const char *firmware, const char *nickname);
extern int nr_devices(const struct device_table *table);
extern const struct device *get_device(const struct device_table *table, int i);
extern void clear_device_nodes(struct device_table *table);
const char *get_dc_nickname(const struct divecomputer *dc);
extern bool device_used_by_selected_dive(const struct device *dev);

extern const struct device *get_device_for_dc(const struct device_table *table, const struct divecomputer *dc);

// struct device accessors for C-code. The returned strings are not stable!
const char *device_get_model(const struct device *dev);
const uint32_t device_get_id(const struct device *dev);
const char *device_get_serial(const struct device *dev);
const char *device_get_firmware(const struct device *dev);
const char *device_get_nickname(const struct device *dev);

#ifdef __cplusplus
}
#endif

// Functions and global variables that are only available to C++ code
#ifdef __cplusplus

#include <string>
#include <vector>
struct device {
	bool operator==(const device &a) const; // TODO: remove, once devices are integrated in the undo system
	bool operator<(const device &a) const;
	void showchanges(const std::string &n, const std::string &s, const std::string &f) const;
	std::string model;
	uint32_t deviceId;
	std::string serialNumber;
	std::string firmware;
	std::string nickName;
};

struct device_table {
	// Keep the dive computers in a vector sorted by (model, deviceId)
	std::vector<device> devices;
};

#endif

#endif // DEVICE_H
