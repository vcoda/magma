namespace magma
{
    template<class DrawCommand>
    class DrawIndirectCommand final
    {
    public:
        explicit DrawIndirectCommand(IndirectBuffer *buffer, void *mappedData) noexcept:
            buffer(buffer)
        {
            MAGMA_ASSERT(buffer->getDrawCount() < buffer->getMaxDrawCount());
            if (buffer->persistentlyMapped())
                drawCmd = reinterpret_cast<DrawCommand *>(mappedData) + buffer->getDrawCount();
            else
            {
                drawCmd = (DrawCommand *)buffer->getMemory()->map(
                    sizeof(DrawCommand) * buffer->getDrawCount(),
                    sizeof(DrawCommand));
                MAGMA_ASSERT(drawCmd);
            }
        }

        ~DrawIndirectCommand()
        {
            if (!buffer->persistentlyMapped())
                buffer->getMemory()->unmap();
        }

        DrawCommand *operator->() { return drawCmd; }
        DrawCommand& operator*() { return *drawCmd; }

        operator bool() const { return drawCmd != nullptr; }

    private:
        IndirectBuffer *const buffer;
        DrawCommand *drawCmd;
    };
} // namespace magma
