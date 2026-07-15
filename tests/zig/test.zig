const std = @import("std");
const assert = std.debug.assert;

const c = @cImport({
    @cInclude("liara/abi_version.h");
});

pub fn main() void {
    const major = c.LIARA_ABI_VERSION_MAJOR;
    const minor = c.LIARA_ABI_VERSION_MINOR;
    const patch = c.LIARA_ABI_VERSION_PATCH;

    assert(major == 0);
    assert(minor == 0);
    assert(patch == 1);

    std.debug.print("Version compile-time: {}.{}.{}\n", .{ major, minor, patch });

    const version_macro = c.LIARA_ABI_VERSION;
    std.debug.print("Version macro: {}\n", .{version_macro});
    std.debug.print(
        "Version macro decoded: {}.{}.{}\n",
        .{
            c.LIARA_VERSION_MAJOR(version_macro),
            c.LIARA_VERSION_MINOR(version_macro),
            c.LIARA_VERSION_PATCH(version_macro),
        },
    );

    var v1: u32 = 0;
    assert(c.liara_try_make_version(1, 2, 3, &v1) == true);
    std.debug.print("make_version(1,2,3) = {}\n", .{v1});

    const v2 = c.liara_abi_version();
    std.debug.print("abi_version() = {}\n", .{v2});

    std.debug.print(
        "decode: {}.{}.{}\n",
        .{
            c.liara_version_major(v1),
            c.liara_version_minor(v1),
            c.liara_version_patch(v1),
        },
    );

    assert(c.liara_version_major(v1) == 1);
    assert(c.liara_version_minor(v1) == 2);
    assert(c.liara_version_patch(v1) == 3);
}
