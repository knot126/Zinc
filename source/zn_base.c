
ZNObject ZNObject_getClass(ZNObject this, ZNClass cls, ZNObject in) {
	return (ZNObject) cls;
}

ZNResult ZNInit(void) {
	zinc.default_class = ZNCreateClass(
		NULL,
		(ZNClassFeild *[]) {
			NULL,
		},
		(ZNClassMethod *[]) {
			&(ZNClassMethod) {"getClass", &ZNObject_getClass, false},
			NULL,
		}
	);
}
