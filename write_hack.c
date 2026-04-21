#include "pes.h"
#include "tree.h"
#include <stdio.h>
#include <string.h>

int object_write(ObjectType type, const void *data, size_t len, ObjectID *id_out);

int main() {
    system("mkdir -p .pes/objects");
    Tree original;
    original.count = 2;

    original.entries[0].mode = 0100644;
    memset(original.entries[0].hash.hash, 0xAA, HASH_SIZE);
    strcpy(original.entries[0].name, "README.md");

    original.entries[1].mode = 0040000;
    memset(original.entries[1].hash.hash, 0xBB, HASH_SIZE);
    strcpy(original.entries[1].name, "src");

    void *data;
    size_t len;
    tree_serialize(&original, &data, &len);

    ObjectID id;
    object_write(OBJ_TREE, data, len, &id);

    char hex[65];
    for (int i = 0; i < HASH_SIZE; i++) {
        sprintf(hex + i * 2, "%02x", id.hash[i]);
    }
    hex[64] = '\0';
    printf(".pes/objects/%.2s/%s\n", hex, hex + 2);
    return 0;
}
