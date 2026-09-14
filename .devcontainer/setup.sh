#!/usr/bin/env bash
set -e

sudo apt-get update
sudo apt-get install -y neovim ripgrep fd-find build-essential clang-format git

NVIM_DIR="$HOME/.config/nvim"
if [ ! -d "$NVIM_DIR" ]; then
  git clone https://github.com/LazyVim/starter "$NVIM_DIR"
  rm -rf "$NVIM_DIR/.git"
fi

mkdir -p "$NVIM_DIR/lua/config"
cat > "$NVIM_DIR/lua/config/options.lua" << 'EOF'

vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true
vim.opt.colorcolumn = "100"
vim.opt.cursorline = true
EOF

mkdir -p "$HOME/.config/clang"
cat > "$HOME/.clang-format" << 'EOF'
IndentWidth: 4
UseTab: Never
TabWidth: 4
ColumnLimit 100
EOF