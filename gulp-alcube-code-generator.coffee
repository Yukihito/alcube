through = require 'through2'
spawn = (require 'child_process').spawn
PluginError = (require 'gulp-util').PluginError
gutil = require 'gulp-util'
PLUGIN_NAME = 'gulp-alcube-code-generator'

module.exports = do ->
  generateDto = (format) ->
    taskOf ['-m', 'codegenerator.main', 'dto', '-f', format]

  generateFunctionPrototypes = ->
    taskOf ['-m', 'codegenerator.main', 'prototypes']

  generateGPUInterfaceHeader = ->
    taskOf ['-m', 'codegenerator.main', 'gpuif_h']

  generateGPUInterfaceCpp = ->
    taskOf ['-m', 'codegenerator.main', 'gpuif_cpp']

  taskOf = (options) ->
    through.obj (file, encoding, callback) ->
      if file.isNull()
        callback()
      else if file.isStream()
        this.emit 'error', new PluginError(PLUGIN_NAME, 'Stream not supported.')
        callback()
      else if file.isBuffer()
        yaml_text = String file.contents
        compiled_text = ''
        cmd = spawn 'python', options
        cmd.stdin.write yaml_text
        cmd.stdin.end()
        cmd.stdout.setEncoding 'utf-8'
        cmd.stdout.on 'data', (data) =>
          compiled_text += data
        cmd.stderr.on 'data', (data) =>
          console.error data.toString()
        cmd.on 'close', (code) =>
          console.log 'Child process "' + options[2] + '" exited with code ' + code
          if code == 0
            output = new gutil.File
              cwd:  file.cwd
              base: file.base
              path: file.path
              contents: new Buffer(compiled_text)
            this.push output
            callback null, output
          else
            callback()
      else
        console.warn 'Unexpected flow'
        callback()

  class CodeGenerator
    @generateDtoCpp: -> generateDto('cpp')
    @generateDtoClc: -> generateDto('clc')
    @generateFunctionPrototypes: -> generateFunctionPrototypes()
    @generateGPUInterfaceHeader: -> generateGPUInterfaceHeader()
    @generateGPUInterfaceCpp: -> generateGPUInterfaceCpp()
  CodeGenerator
