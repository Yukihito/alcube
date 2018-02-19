through = require 'through2'
spawn = (require 'child_process').spawn
PluginError = (require 'gulp-util').PluginError
gutil = require 'gulp-util'
PLUGIN_NAME = 'gulp-alcube-code-generator'

module.exports = do ->
  generateDto = (format) ->
    taskOf 'python', ['-m', 'codegenerator.main', 'dto', '-f', format]

  generateFunctionPrototypes = ->
    taskOf 'python', ['-m', 'codegenerator.main', 'prototypes']

  generateGPUInterfaceHeader = ->
    taskOf 'python', ['-m', 'codegenerator.main', 'gpuif_h']

  generateGPUInterfaceCpp = ->
    taskOf 'python', ['-m', 'codegenerator.main', 'gpuif_cpp']

  compileKernel = ->
    taskOf './cmake-build-release/kernel_compiler', []

  taskOf = (command, options) ->
    through.obj (file, encoding, callback) ->
      if file.isNull()
        callback()
      else if file.isStream()
        this.emit 'error', new PluginError(PLUGIN_NAME, 'Stream not supported.')
        callback()
      else if file.isBuffer()
        yaml_text = String file.contents
        compiled_text = ''
        cmd = spawn command, options
        cmd.stdin.write yaml_text
        cmd.stdin.end()
        cmd.stdout.setEncoding 'utf-8'
        cmd.stdout.on 'data', (data) =>
          compiled_text += data
        cmd.stderr.on 'data', (data) =>
          console.error data.toString()
        cmd.on 'close', (code) =>
          self = command + ' ' + options.join(' ')
          console.log 'Child process "' + self + '" exited with code ' + code
          if code == 0
            output = new gutil.File
              cwd:  file.cwd
              base: file.base
              path: file.path
              contents: new Buffer(compiled_text)
            this.push output
            callback null, output
          else
            callback('Abort on executing "' + self + '"')
      else
        console.warn 'Unexpected flow'
        callback()

  class CodeGenerator
    @generateDtoCpp: -> generateDto('cpp')
    @generateDtoClc: -> generateDto('clc')
    @generateFunctionPrototypes: -> generateFunctionPrototypes()
    @generateGPUInterfaceHeader: -> generateGPUInterfaceHeader()
    @generateGPUInterfaceCpp: -> generateGPUInterfaceCpp()
    @compileKernel: -> compileKernel()
  CodeGenerator
