#include "ClientInstance.h"

void LevelRenderer::reloadChunks() {
	using rebuildAllRenderChunkGeometry_t = void(__fastcall*)(__int64, __int64, __int64);
	static rebuildAllRenderChunkGeometry_t rebuildAllRenderChunkGeometry = reinterpret_cast<rebuildAllRenderChunkGeometry_t>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 48 ? ? ? ? 48 8B F9 48 8D A9"));
	if (!this)
		return;
	unsigned long long* v5;  // rdi
	unsigned long long* i;   // rbx

	v5 = *(unsigned long long**)((char*)this + 32);
	for (i = (unsigned long long*)*v5; i != v5; i = (unsigned long long*)*i)
		rebuildAllRenderChunkGeometry(i[3], 0, 0);
}
