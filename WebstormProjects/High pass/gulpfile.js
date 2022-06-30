const { src, dest, series, watch } = require('gulp')

const sass =  require('gulp-sass')(require('sass'))

const toScss = () => {
  return src('src/scss/main-scss.scss')
    .pipe(sass().on('error', sass.logError))
    .pipe(gulp.dest('src/css/main.css'))
}

watch('src/scss/main-scss.scss', toScss)

exports.toScss = toScss
exports.dev = series(toScss)