types = [
	["Pointer", "void *"],
	# ["Object", "ZNObject"],
	["Int8", "int8_t"],
	["UInt8", "uint8_t"],
	["Int16", "int16_t"],
	["UInt16", "uint16_t"],
	["Int32", "int32_t"],
	["UInt32", "uint32_t"],
	["Int64", "int64_t"],
	["UInt64", "uint64_t"],
	["Float32", "float"],
	["Float64", "double"],
	["String", "const char *"]
]

for t in types:
	print(f"\t{t[1]} as{t[0]};")

toX = """#define ZNTo{Tfull}(p) ((ZNStorable) {{.asObject = p}}).as{Tfull}
#define ZNFrom{Tfull}(p) ((ZNStorable) {{.as{Tfull} = p}}).asObject"""

for t in types:
	print(toX.format(Tfull = t[0]))
