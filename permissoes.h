#ifndef PERMISSOES_H
#define PERMISSOES_H

// ── Bits de permissão ────────────────────────────────────────────────────────
// Cada permissão ocupa 1 bit. O | (OR) combina permissões num perfil.
#define PERM_LISTAR    0x01   // bit 0 → 00001
#define PERM_CADASTRAR 0x02   // bit 1 → 00010
#define PERM_RESERVAR  0x04   // bit 2 → 00100
#define PERM_VENDER    0x08   // bit 3 → 01000
#define PERM_ADMIN     0x10   // bit 4 → 10000

// ── Perfis de usuário ────────────────────────────────────────────────────────
#define PERFIL_MECANICO (PERM_LISTAR)
#define PERFIL_VENDEDOR (PERM_LISTAR | PERM_RESERVAR | PERM_VENDER)
#define PERFIL_GERENTE  (PERM_LISTAR | PERM_CADASTRAR | PERM_RESERVAR | PERM_VENDER | PERM_ADMIN)

void verificarPermissoes();

#endif
