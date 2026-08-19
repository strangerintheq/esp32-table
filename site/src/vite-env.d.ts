/// <reference types="vite/client" />
/// <reference types="preact" />

// Tell TypeScript that side-effect CSS imports are valid modules
declare module '*.css' {
    const content: { [className: string]: string };
    export default content;
}