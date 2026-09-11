#!/usr/bin/env bash
set -euo pipefail

# --- Configuration --------------------------------------------------------
# Central place for anything that might need to change later.
CLANG_FORMAT_BIN="${CLANG_FORMAT_BIN:-clang-format}"
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CLANG_FORMAT_STYLE_ARG="--style=file"
FORMAT_EXTENSIONS=("cpp" "hpp" "cc")
EXCLUDE_DIRS=("${REPO_ROOT}/libs/vendor" "${REPO_ROOT}/build")
COLOR_BLUE="$(tput setaf 4 2>/dev/null || true)"
COLOR_WHITE="$(tput setaf 7 2>/dev/null || true)"
COLOR_GREEN="$(tput setaf 2 2>/dev/null || true)"
COLOR_RED="$(tput setaf 1 2>/dev/null || true)"
COLOR_RESET="$(tput sgr0 2>/dev/null || true)"
# ---------------------------------------------------------------------------

CHECK_MODE=0
VERBOSE=0
TARGET=""

usage() {
    echo "Usage: $(basename "${BASH_SOURCE[0]}") [--check] [--verbose] [path]"
    echo
    echo "  path            A single file or a directory to format recursively."
    echo "                  Defaults to the whole repository if omitted."
    echo "  --check         Report files that need formatting instead of rewriting them."
    echo "  -v, --verbose   Print what the script is doing."
}

log() {
    if [[ "$VERBOSE" -eq 1 ]]; then
        echo "[verbose] $*" >&2
    fi
}

parse_args() {
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --check)
                CHECK_MODE=1
                shift
                ;;
            -v | --verbose)
                VERBOSE=1
                shift
                ;;
            -h | --help)
                usage
                exit 0
                ;;
            -*)
                echo "Unknown option: $1" >&2
                usage
                exit 1
                ;;
            *)
                if [[ -n "$TARGET" ]]; then
                    echo "Only one path may be specified." >&2
                    usage
                    exit 1
                fi
                TARGET="$1"
                shift
                ;;
        esac
    done

    if [[ -z "$TARGET" ]]; then
        TARGET="$REPO_ROOT"
    fi
}

check_clang_format_available() {
    if ! command -v "$CLANG_FORMAT_BIN" >/dev/null 2>&1; then
        echo "Error: '$CLANG_FORMAT_BIN' not found on PATH." >&2
        exit 1
    fi
}

is_valid_extension() {
    local path="$1"
    local ext="${path##*.}"
    local candidate
    for candidate in "${FORMAT_EXTENSIONS[@]}"; do
        if [[ "$ext" == "$candidate" ]]; then
            return 0
        fi
    done
    return 1
}

build_name_expr() {
    local expr=()
    local ext
    for ext in "${FORMAT_EXTENSIONS[@]}"; do
        if [[ ${#expr[@]} -gt 0 ]]; then
            expr+=(-o)
        fi
        expr+=(-name "*.${ext}")
    done
    printf '%s\n' "${expr[@]}"
}

build_prune_expr() {
    local expr=()
    local dir
    for dir in "${EXCLUDE_DIRS[@]}"; do
        if [[ ${#expr[@]} -gt 0 ]]; then
            expr+=(-o)
        fi
        expr+=(-path "$dir")
    done
    printf '%s\n' "${expr[@]}"
}

collect_files() {
    local target="$1"

    if [[ -f "$target" ]]; then
        if ! is_valid_extension "$target"; then
            echo "Error: '$target' does not have a recognized extension (${FORMAT_EXTENSIONS[*]})." >&2
            return 1
        fi
        echo "$target"
        return
    fi

    if [[ -d "$target" ]]; then
        local name_expr=()
        local prune_expr=()
        mapfile -t name_expr < <(build_name_expr)
        mapfile -t prune_expr < <(build_prune_expr)

        if [[ ${#prune_expr[@]} -gt 0 ]]; then
            log "Excluding: ${EXCLUDE_DIRS[*]}"
            find "$target" \( "${prune_expr[@]}" \) -prune -o -type f \( "${name_expr[@]}" \) -print
        else
            find "$target" -type f \( "${name_expr[@]}" \) -print
        fi
        return
    fi

    echo "Error: '$target' is not a file or directory." >&2
    return 1
}

run_check() {
    local files=("$@")
    local failed=()
    local file

    for file in "${files[@]}"; do
        if "$CLANG_FORMAT_BIN" --dry-run --Werror "$CLANG_FORMAT_STYLE_ARG" "$file" >/dev/null 2>&1; then
            log "Checking $file ... ${COLOR_GREEN}OK${COLOR_RESET}"
        else
            log "Checking $file ... ${COLOR_RED}needs formatting${COLOR_RESET}"
            failed+=("$file")
        fi
    done

    if [[ ${#failed[@]} -gt 0 ]]; then
        echo "The following files need formatting:"
        printf '  %s\n' "${failed[@]}"
        return 1
    fi

    echo "All ${#files[@]} file(s) are properly formatted."
    return 0
}

run_format() {
    local files=("$@")

    if [[ ${#files[@]} -eq 0 ]]; then
        echo "No matching files found."
        return 0
    fi

    local file
    for file in "${files[@]}"; do
        log "${COLOR_BLUE}Formatting${COLOR_RESET} ${COLOR_WHITE}$file${COLOR_RESET}"
    done

    "$CLANG_FORMAT_BIN" -i "$CLANG_FORMAT_STYLE_ARG" "${files[@]}"
    echo "Formatted ${#files[@]} file(s)."
}

main() {
    parse_args "$@"
    log "Mode: $([[ "$CHECK_MODE" -eq 1 ]] && echo check || echo format)"
    log "Target: $TARGET"
    check_clang_format_available

    local collected
    if ! collected="$(collect_files "$TARGET")"; then
        exit 1
    fi

    local files=()
    if [[ -n "$collected" ]]; then
        mapfile -t files <<<"$collected"
    fi

    if [[ ${#files[@]} -eq 0 ]]; then
        echo "No matching files found under '$TARGET'."
        exit 0
    fi

    log "Found ${#files[@]} file(s)"

    if [[ "$CHECK_MODE" -eq 1 ]]; then
        run_check "${files[@]}"
    else
        run_format "${files[@]}"
    fi
}

main "$@"
