import java.io.File
import java.io.IOException
import java.nio.file.Paths

class Sexp(var value: String, var parent: Sexp? = null,
           var children: List<Sexp> = ArrayList()) {

  operator fun plusAssign(child: Sexp) {
    children += child
    child.parent = this
  }

  override fun toString(): String {
    fun tab(): String {
      var curr: Sexp? = this; var res = 0
      while (curr != null) {
        res++
        curr = curr.parent
      }
      return (0 until res - 1).joinToString("") { "  " }
    }

    if (children.isEmpty()) return tab() + value + "\n"

    return tab() + value + "\n" + children.joinToString("")
  }
}

class Reader(file: File) {
  val array = ArrayList<Char>(file.readBytes().map { it.toChar() })
  var index = 0

  fun getc(): Char {
    println(array[index])
    return array[index++]
  }

  fun peekc(): Char {
    return array[index]
  }

  fun hasNext(): Boolean {
    return index != array.size
  }

  override fun toString(): String {
    return array
        .subList(index, array.size)
        .joinToString("")
  }

  fun prevc(): Char {
    return array[index - 1]
  }
}

val programName = "hello.kl"

fun main(args: Array<String>) {
  val file = Paths.get("../examples/" + programName).toFile()
  val sc = Reader(file)
  val program = pProgram(sc)
  println(program)
}

fun pProgram(r: Reader): Sexp {
  val program = Sexp(programName)
  while (r.hasNext()) {
    program += pSexp(r)
    pWhitespace(r)
  }
  return program
}

fun pSexp(r: Reader): Sexp {
  pWhitespace(r)
  print("${r.peekc()}: ")
  if (r.peekc() == '(')
    return pList(r)
  else
    return pAtom(r)
}

fun pList(r: Reader): Sexp {
  println("parse list")
  if (r.getc() != '(') throw IOException("expecting l-paren while parsing list")
  val curr = Sexp(pWord(r))

  while (r.peekc() != ')') {
    curr += pSexp(r)
  }
  print("${r.getc()}: end of list\n")
  return curr
}

fun pAtom(r: Reader): Sexp {
  println("parse atom")

  if (r.peekc() == '\'') {
    return pChar(r)
  } else if (r.peekc() == '"') {
    return pString(r)
  } else {
    return Sexp(pWord(r))
  }
}

fun pWord(r: Reader): String {
  var acc = ""
  while (r.peekc() != '('
      && r.peekc() != ')'
      && !Character.isWhitespace(r.peekc())) {
    acc += r.getc()
  }
  return acc
}

fun pChar(r: Reader): Sexp {
  var acc = ""
  acc += r.getc()
  while (r.peekc() != '\'' && r.prevc() != '\\') {
    acc += r.getc()
  }
  acc += r.getc()
  return Sexp(acc)
}

fun pString(r: Reader): Sexp {
  var acc = ""
  acc += r.getc()
  while (r.peekc() != '"' && r.prevc() != '\\') {
    acc += r.getc()
  }
  if (r.peekc() != '"') throw IOException("shouldn't happen")
  acc += r.getc()
  return Sexp(acc)
}

fun pWhitespace(r: Reader) {
  while (r.hasNext() && Character.isWhitespace(r.peekc()))
    r.getc()
}
