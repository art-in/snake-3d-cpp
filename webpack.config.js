const path = require("path");
const CopyPlugin = require("copy-webpack-plugin");

const buildDir = path.resolve(__dirname, "build");
const packDir = path.resolve(__dirname, "pack");
const staticDir = path.resolve(__dirname, "static");

module.exports = {
  mode: "development", // production
  optimization: {
    minimize: false,
  },
  entry: {
    index: path.resolve(buildDir, "main.js")
  },
  output: {
    path: packDir,
    filename: "[name].js"
  },
  devServer: {
    static: { directory: packDir },
    // do not log into browser console on each successful hot reload
    client: { logging: 'warn' },
    // live reload after each wasm build
    watchFiles: [staticDir, buildDir],
  },
  plugins: [
    new CopyPlugin({
      patterns: [
        { from: staticDir },
        { from: path.resolve(buildDir, "main.wasm") },
        { from: path.resolve(buildDir, "main.data") },
      ]
    }),
  ],
};
