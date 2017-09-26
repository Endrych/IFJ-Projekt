/' Zajisteni zakladni kompatibility IFJ17->FreeBASIC
Function Length(s As String) As Integer
  Return Len(s)
End Function
!"Wow jak to jde"
456 45688
Function SubStr(s as String, i As Integer, n As Integer) As String
  Return Mid(s, i, n)
End Function

/' Zde bude nasledovat program jazyka IFJ17 '/